#include "FrameProcessor.h"

namespace QrScanner
{
	FrameProcessor::FrameProcessor(QObject *parent)
		: QObject(parent)
		, m_videoSink(nullptr)
	{
	}

	FrameProcessor::~FrameProcessor()
	{
	}

	QVideoSink *FrameProcessor::videoSink() const
	{
		return m_videoSink;
	}

	void FrameProcessor::setVideoSink(QVideoSink *sink)
	{
		if (m_videoSink != sink)
		{
			m_videoSink = sink;
			connect(m_videoSink, &QVideoSink::videoFrameChanged, this, &FrameProcessor::processFrame);
		}
	}

	void FrameProcessor::processFrame(const QVideoFrame &frame)
	{
		emit frameReady(frame);
	}

}