#pragma once

#include <taglib/tag.h>
#include <taglib/tstringlist.h>

#include <QByteArray>
#include <QFlags>
#include <QImage>
#include <QString>

#if defined(__EXTRA_METADATA__)
// UUID -> QString
#include "util/compatibility.h"
#endif // __EXTRA_METADATA__

#include "track/trackmetadata.h"

namespace mixxx {

namespace taglib {

QString toQString(
        const TagLib::String& tString);

TagLib::String toTString(
        const QString& qString);

/// Returns the first element of TagLib string list that is not empty.
TagLib::String firstNonEmptyStringListItem(
        const TagLib::StringList& strList);

/// Returns a QByteArray that owns the data.
inline QByteArray toQByteArray(
        const TagLib::ByteVector& tByteVector) {
        return QByteArray(
                tByteVector.data(),
                tByteVector.size());
}

/// Returns a QByteArray that directly accesses the underlying byte vector!
inline QByteArray toQByteArrayRaw(
        const TagLib::ByteVector& tByteVector) {
        return QByteArray::fromRawData(
                tByteVector.data(),
                tByteVector.size());
}

inline TagLib::ByteVector toTByteVector(
        const QByteArray& byteArray) {
    if (byteArray.isNull()) {
        return TagLib::ByteVector();
    } else {
        return TagLib::ByteVector(byteArray.constData(), byteArray.size());
    }
}

#if defined(__EXTRA_METADATA__)
inline TagLib::String uuidToTString(
        const QUuid& uuid) {
    return toTString(uuidToNullableStringWithoutBraces(uuid));
}
#endif // __EXTRA_METADATA__

inline QString formatBpm(
        const TrackMetadata& trackMetadata) {
    const Bpm bpm = trackMetadata.getTrackInfo().getBpm();
    if (!bpm.isValid()) {
        return {};
    }
    return Bpm::valueToString(bpm.value());
}

bool parseBpm(
        TrackMetadata* pTrackMetadata,
        const QString& sBpm);

inline QString formatReplayGainGain(
        const ReplayGain& replayGain) {
    return ReplayGain::ratioToString(replayGain.getRatio());
}

inline QString formatReplayGainPeak(
        const ReplayGain& replayGain) {
    return ReplayGain::peakToString(replayGain.getPeak());
}

inline QString formatTrackGain(
        const TrackMetadata& trackMetadata) {
    return formatReplayGainGain(
            trackMetadata.getTrackInfo().getReplayGain());
}

bool parseTrackGain(
        TrackMetadata* pTrackMetadata,
        const QString& dbGain);

inline QString formatTrackPeak(
        const TrackMetadata& trackMetadata) {
    return formatReplayGainPeak(
            trackMetadata.getTrackInfo().getReplayGain());
}

bool parseTrackPeak(
        TrackMetadata* pTrackMetadata,
        const QString& strPeak);

#if defined(__EXTRA_METADATA__)
inline QString formatAlbumGain(
        const TrackMetadata& trackMetadata) {
    return formatReplayGainGain(trackMetadata.getAlbumInfo().getReplayGain());
}

bool parseAlbumGain(
        TrackMetadata* pTrackMetadata,
        const QString& dbGain);

inline QString formatAlbumPeak(
        const TrackMetadata& trackMetadata) {
    return formatReplayGainPeak(trackMetadata.getAlbumInfo().getReplayGain());
}

bool parseAlbumPeak(
        TrackMetadata* pTrackMetadata,
        const QString& strPeak);
#endif // __EXTRA_METADATA__

bool parseSeratoBeatGrid(
        TrackMetadata* pTrackMetadata,
        const QByteArray& data,
        FileType fileType);

bool parseSeratoBeatGrid(
        TrackMetadata* pTrackMetadata,
        const TagLib::String& data,
        FileType fileType);

bool parseSeratoMarkers(
        TrackMetadata* pTrackMetadata,
        const QByteArray& data,
        FileType fileType);

bool parseSeratoMarkers(
        TrackMetadata* pTrackMetadata,
        const TagLib::String& data,
        FileType fileType);

bool parseSeratoMarkers2(
        TrackMetadata* pTrackMetadata,
        const QByteArray& data,
        FileType fileType);

bool parseSeratoMarkers2(
        TrackMetadata* pTrackMetadata,
        const TagLib::String& data,
        FileType fileType);

TagLib::String dumpSeratoBeatGrid(
        const TrackMetadata& trackMetadata,
        FileType fileType);

TagLib::String dumpSeratoMarkers(
        const TrackMetadata& trackMetadata,
        FileType fileType);

TagLib::String dumpSeratoMarkers2(
        const TrackMetadata& trackMetadata,
        FileType fileType);

inline QImage loadImageFromByteVector(
        const TagLib::ByteVector& imageData,
        const char* format = nullptr) {
    return QImage::fromData(
            // char -> uchar
            reinterpret_cast<const uchar*>(imageData.data()),
            imageData.size(),
            format);
}

/// Bitmask of optional tag fields that should NOT be read from the
/// common part of the tag through TagLib::Tag.
/// Usage: The write functions for ID3v2, MP4, APE and XiphComment tags
/// have specialized code for some or all of the corresponding tag fields
/// and the common implementation sometime doesn't work as expected.
enum class ReadTagFlag {
    OmitNone = 0,
    OmitComment = 1 << 0,
};

Q_DECLARE_FLAGS(ReadTagMask, ReadTagFlag)

Q_DECLARE_OPERATORS_FOR_FLAGS(ReadTagMask)

void importTrackMetadataFromTag(
        TrackMetadata* pTrackMetadata,
        const TagLib::Tag& tag,
        ReadTagMask readMask = ReadTagFlag::OmitNone);

/// Bitmask of optional tag fields that should NOT be written into the
/// common part of the tag through TagLib::Tag. For future extension
/// it is safer to explicitly specify these exceptions!
/// Usage: The write functions for ID3v2, MP4, APE and XiphComment tags
/// have specialized code for some or all of the corresponding tag fields
/// and it is not needed or even dangerous to use the common setters of
/// TagLib::Tag.
enum class WriteTagFlag {
    OmitNone = 0,
    OmitComment = 1 << 0,
    OmitTrackNumber = 1 << 1,
    OmitYear = 1 << 2,
};

Q_DECLARE_FLAGS(WriteTagMask, WriteTagFlag)

Q_DECLARE_OPERATORS_FOR_FLAGS(WriteTagMask)

void exportTrackMetadataIntoTag(
        TagLib::Tag* pTag,
        const TrackMetadata& trackMetadata,
        WriteTagMask writeMask);

} // namespace taglib

} // namespace mixxx
