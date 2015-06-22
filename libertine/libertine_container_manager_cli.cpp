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

  std::cout << "Default container is " << containers->default_container_id().toStdString().c_str() << std::endl;

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
    if (argc > 2)
    {
      printf("Too many arguments\n");
    }
    QVariantMap image;
    image.insert("id", "wily");
    image.insert("name", "Ubuntu 'Wily Werewolf'");
    containers->addNewContainer(image);
    printf("Create is set\n");
  }

  if (commandlineParser.isSet("destroy"))
  {
    if (commandlineParser.isSet("name"))
    {
      const QString container_name = commandlineParser.value("name");
      printf("Destroy is set w/ value %s\n", container_name.toStdString().c_str());
    }
    else
    {
      printf("You must specify a container name when using the destroy option!\n");
    }
  }

  if (commandlineParser.isSet("install-package"))
  {
    if (commandlineParser.isSet("name"))
    {
      const QString package_name = commandlineParser.value("install-package");
      const QString container_name = commandlineParser.value("name");
      printf("install-package is set w/ values %s & %s\n", package_name.toStdString().c_str(), container_name.toStdString().c_str());
    }
    else
    {
      printf("You must specify a container name when using the install-package option!\n");
    }
  }

  if (commandlineParser.isSet("update"))
  {

  }

  if (commandlineParser.isSet("list"))
  {
    int count = containers->size();
    QVariant name;
    printf("size is %d\n", count);

    for (int i = 0; i < count; ++i)
    {
      name = containers->data(containers->index(i, 0), (int)ContainerConfigList::DataRole::ContainerName);
      printf("name is %s\n", name.toString().toStdString().c_str());
    }
  }

  return 0;
}
