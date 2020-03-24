#pragma once


#include<opencv2/core/core.hpp>
#include<opencv2/calib3d/calib3d.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<thread>

static void DisplayImage(cv::Mat *frame)
{
	cv::namedWindow("Frame Checking", cv::WINDOW_NORMAL);
	imshow("Frame Checking", *frame);
	cv::waitKey(1);
}

static void clearBuffer(cv::VideoCapture *const capture,
	cv::Mat *const frame,
	bool *const startCapture,
	bool *const stopThread,
	std::mutex &FrameLock)
{
	while (*stopThread == false)
	{
		if (*startCapture == false)
		{
			FrameLock.lock();
			capture->grab();
			//*capture >> *frame;
			FrameLock.unlock();

			//DisplayImage(frame);
		}
	}

	std::cout << "end..." << std::endl;
}

class camera
{

public:

	camera(const std::string & path, const int index):
		m_imagePath  { path },
		m_index  { index },
		m_stopCapture {false},
		m_stopThread {false},
		m_purgeBufferThread()
	{
	}

	~camera()
	{
		m_stopThread = true;
		m_purgeBufferThread.join();
	}

	bool startCapture()
	{

		if (!m_capture.open(m_index)) return false;

		m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 2592);
		m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1944);


		//m_capture.set(cv::CAP_PROP_EXPOSURE, -5);
		//m_capture.set(cv::CAP_PROP_BUFFERSIZE, 1);

		// load thread to clear buffer
		m_purgeBufferThread = std::thread(clearBuffer, &m_capture, &m_frame, &m_stopCapture, &m_stopThread, std::ref(m_captureFrame));
		return true;
	}

	void stopCapture()
	{
		m_stopCapture = true;
		m_capture.release();
	}


	void storeImage(int level)
	{
		m_captureFrame.lock();
		m_capture.retrieve(m_image);
		m_captureFrame.unlock();

		char buffer[8];
		snprintf(buffer, sizeof(buffer), "%d.jpg", level);
		std::string tmp = m_imagePath + buffer;
		std::cout << "test:" << tmp << std::endl;

		cv::imwrite(tmp.c_str(), m_image);
	}

private:

	std::string m_imagePath;
	int m_index;

	std::thread m_purgeBufferThread;
	cv::Mat m_frame, m_image;
	cv::VideoCapture m_capture;
	bool m_stopCapture = false;
	bool m_stopThread = false;
	std::mutex m_captureFrame;
};
