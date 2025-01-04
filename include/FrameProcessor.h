#pragma once

#include <QObject>
#include <QVideoSink>
#include <QVideoFrame>
#include <QDebug>


namespace QrScanner
{
	class FrameProcessor : public QObject
	{
		Q_OBJECT

	public:
		FrameProcessor(QObject *parent = nullptr);
		~FrameProcessor();

		QVideoSink *videoSink() const;
		void setVideoSink(QVideoSink *sink);

		signals:
			void frameReady(const QVideoFrame &frame);

		private slots:
			void processFrame(const QVideoFrame &frame);

		private:
			QVideoSink *m_videoSink;
	};
} // namespace QrScanner