$script = <<SCRIPT
set -e

apt-add-repository -y ppa:brightbox/ruby-ng

sudo apt-get update
sudo apt-get install -y build-essential \
                        graphicsmagick \
                        libgraphicsmagick++1-dev \
                        libmagickwand-dev \
                        ruby2.2 \
                        ruby2.2-dev

sudo update-alternatives --set ruby /usr/bin/ruby2.2
sudo update-alternatives --set gem /usr/bin/gem2.2

sudo gem install bundler

cd /vagrant
bundle install
rake
SCRIPT

Vagrant.configure(2) do |config|

  config.vm.box = "ubuntu/trusty64"

  config.vm.network "forwarded_port", guest: 5000, host: 5000

  config.vm.provider "virtualbox" do |vb|
    vb.gui = false
    vb.cpus   = 2
    vb.memory = 4096
  end

  config.vm.provision :shell, inline: $script, keep_color: true
end
