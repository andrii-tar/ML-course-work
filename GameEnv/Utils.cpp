#include"Utils.h"

void writeVectorToCSV(const std::vector<int>& data, const std::string& filename) {
	std::ofstream file(filename, std::ios_base::app);

	if (file.is_open()) {
		for (size_t i = 0; i < data.size(); ++i) {
			file << data[i];

			// Add comma if it's not the last element
			if (i != data.size() - 1)
				file << ",";
		}
		file << std::endl;
		file.close();
		//std::cout << "Data has been written to " << filename << " successfully." << std::endl;
	}
	else {
		std::cerr << "Unable to open file " << filename << " for writing." << std::endl;
	}
}

void plotGameScores(vector<int> scrPlayer1, vector<int>scrPlayer2, vector<int>iters, string title)
{
	auto axes = CvPlot::makePlotAxes();
	axes.create<CvPlot::Series>(iters, scrPlayer1, "-r");
	axes.create<CvPlot::Series>(iters, scrPlayer2, "-b");

	axes.xLabel("Iteration");
	axes.yLabel("Score");

	axes.title(title);
	CvPlot::show("Game scores", axes);

	cv::waitKey();
}