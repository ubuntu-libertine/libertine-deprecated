/**
 * @file libertine_container_manager_cli.cpp
 * @brief The CLI manager Libertine Containers
 */
/*
 * Copyright 2015 Canonical Ltd
 *
 * Libertine is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3, as published by the
 * Free Software Foundation.
 *
 * Libertine is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "libertine_lxc_manager_wrapper.h"
#include "ContainerConfigList.h"
#include "libertine/LibertineConfig.h"
#include "libertine/config.h"

#include <getopt.h>
#include <iostream>
#include <unistd.h>
#include <QCoreApplication>
#include <QtCore/QCommandLineParser>

int main (int argc, char *argv[])
{
  LibertineConfig config;
  ContainerConfigList* containers;
  QCommandLineParser commandlineParser;

  QCoreApplication app(argc, argv);
  app.setApplicationName(LIBERTINE_APPLICATION_NAME);

  containers = new ContainerConfigList(&config);

  commandlineParser.setApplicationDescription("Command-line tool to manage sandboxes for running legacy DEB-packaged X11-based applications");
  commandlineParser.addHelpOption();
  commandlineParser.addOptions({
      {{"c", "create"},
        QCoreApplication::translate("main", "Create a new Libertine container.")},
      {{"d", "destroy"},
        QCoreApplication::translate("main", "Destroy an existing Libertine container.")},
      {{"i", "install-package"},
        QCoreApplication::translate("main", "Install a specified Debian package in the Libertine container."),
        QCoreApplication::translate("main", "package_name")},
      {{"u", "update"},
        QCoreApplication::translate("main", "Update the packages in the Libertine container.")},
      {{"l", "list"},
        QCoreApplication::translate("main", "List all available Libertine containers.")},
      {{"n", "name"},
        QCoreApplication::translate("main", "Name of the container to perform the action on."),
        QCoreApplication::translate("main", "container_name")}
  });
  commandlineParser.process(app);

  if (commandlineParser.isSet("create"))
  {
    if (argc != 2)
    {
      std::cout << QCoreApplication::translate("main", "Wrong number of arguments specified.").toStdString().c_str() << std::endl;
      commandlineParser.showHelp(-1);
    }

    QVariantMap image;
    image.insert("id", "wily");
    image.insert("name", "Ubuntu 'Wily Werewolf'");
    containers->addNewContainer(image);
    printf("Create is set\n");
  }

  if (commandlineParser.isSet("destroy"))
  {
    if (argc != 3)
    {
      std::cout << QCoreApplication::translate("main", "Wrong number of arguments specified.").toStdString().c_str() << std::endl;
      commandlineParser.showHelp(-1);
    }

    if (commandlineParser.isSet("name"))
    {
      const QString container_name = commandlineParser.value("name");
      printf("Destroy is set w/ value %s\n", container_name.toStdString().c_str());
    }
    else
    {
      std::cout << QCoreApplication::translate("main", "You must specify a container name when using the destroy option!").toStdString().c_str() << std::endl;
      commandlineParser.showHelp(-1);
    }
  }

  if (commandlineParser.isSet("install-package"))
  {
    if (argc != 3)
    {
      std::cout << QCoreApplication::translate("main", "Wrong number of arguments specified.").toStdString().c_str() << std::endl;
      commandlineParser.showHelp(-1);
    }

    if (commandlineParser.isSet("name"))
    {
      const QString package_name = commandlineParser.value("install-package");
      const QString container_name = commandlineParser.value("name");
      printf("install-package is set w/ values %s & %s\n", package_name.toStdString().c_str(), container_name.toStdString().c_str());
    }
    else
    {
      std::cout << QCoreApplication::translate("main", "You must specify a container name when using the install-package option!").toStdString().c_str() << std::endl;
      commandlineParser.showHelp(-1);
    }
  }

  if (commandlineParser.isSet("update"))
  {
    if (argc != 3)
    {
      std::cout << QCoreApplication::translate("main", "Wrong number of arguments specified.").toStdString().c_str() << std::endl;
      commandlineParser.showHelp(-1);
    }

    if (commandlineParser.isSet("name"))
    {
      const QString container_name = commandlineParser.value("name");
      printf("Update is set w/ value %s\n", container_name.toStdString().c_str());
    }
    else
    {
      std::cout << QCoreApplication::translate("main", "You must specify a container name when using the update option!").toStdString().c_str() << std::endl;
      commandlineParser.showHelp(-1);
    }
  }

  if (commandlineParser.isSet("list"))
  {
    if (argc != 2)
    {
      std::cout << QCoreApplication::translate("main", "Wrong number of arguments specified.").toStdString().c_str() << std::endl;
      commandlineParser.showHelp(-1);
    }

    int count = containers->size();
    QVariant name;

    for (int i = 0; i < count; ++i)
    {
      name = containers->data(containers->index(i, 0), (int)ContainerConfigList::DataRole::ContainerName);
      printf("name is %s\n", name.toString().toStdString().c_str());
    }
  }

  return 0;
}
