// SPDX-License-Identifier: BSD-3-Clause

#include <QCoreApplication>
#include <QCommandLineParser>

#include "ImgAcquisitionApp.hpp"
#include "Settings.hpp"  

int main(int argc, char* argv[])
{
    // Create a QCoreApplication for command-line processing
    QCoreApplication coreApp(argc, argv);
    
    QCommandLineParser parser;
    parser.setApplicationDescription("Image Acquisition Application");
    parser.addHelpOption();
    
    QCommandLineOption configOption(QStringList() << "c" << "config",
                                    "Specify a custom config file path",
                                    "config");
    parser.addOption(configOption);
    parser.process(coreApp);
    
    // If a custom config file is specified, set it in Settings
    if (parser.isSet(configOption)) {
        QString configPath = parser.value(configOption);
        Settings::setCustomConfigFile(configPath.toStdString());
    }
    
    ImgAcquisitionApp app(argc, argv);
    return app.exec();
}
