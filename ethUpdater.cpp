#include <iostream>
#include <stdexcept>
#include <cstring>
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;
string getEthVer(); //Get the latest version of Ethereum
void getEthSrc(string version); //Get the latest version of Ethereum source code.
void clearOldEth(); //Clear the old Ethereum version
void unTmp(string version); //Unzip the temporary folder to compile Ethereum
void compileEth(string version); //Compile the latest Ethereum
void moveEth(string version); //Replace the old Ethereum
void delTmp(string version); //Remove the temporary folder that used to compile Ethereum
string getEpiVer(); //Get the latest version of Epirus
void getEpiSrc(string version); //Get the latest version of Epirus
void crEpiTmp(string version); //Create the temporary folder to hold the Epirus setup file
void moveEpi(string version); //Replace the previous Epirus.
void delEpiTmp(string version); //Remove the temporary folder for Epirus.
string getSolcVer(); //Get the latest version of Solidity
void getSolc(string version); //Download the Solidity executable
void delOldSolc(); //Remove the old Solidity
void moveSolc(); //Replace the updated Solidity

int main()
{
	string ethVersion = getEthVer();
	if (ethVersion.empty())
	{
		cout << "Cannot Get Ethereum Version, update terminated!" << endl;
		exit(1);
	}
	getEthSrc(ethVersion);
	clearOldEth();
	unTmp(ethVersion);
	compileEth(ethVersion);
	moveEth(ethVersion);
	delTmp(ethVersion);
	string epiVersion = getEpiVer();
	if (epiVersion.empty())
	{
		cout << "Cannot Get Epirus Version, update terminated!" << endl;
		exit(1);
	}
	getEpiSrc(epiVersion);
	crEpiTmp(epiVersion);
	moveEpi(epiVersion);
	delEpiTmp(epiVersion);
	string solcVersion = getSolcVer();
	if (solcVersion.empty())
	{
		cout << "Cannot Get Solc Version, update terminated!" << endl;
	}
	getSolc(solcVersion);
	delOldSolc();
	moveSolc();
	system("clear");
	cout << "Remember to add the installation path of Ethereum to your PATH variables in .bashrc" << endl;
	cout << "Please ignore this message if you are upgrading Ethereum" << endl;
}

string getSolcVer()
{
	string orCmd = "curl https://github.com/ethereum/solidity/releases/latest";
	string orResult = "";
	char buffer[128];

	FILE* pipe = popen(orCmd.c_str(), "r");
	if (!pipe)
	{
		throw std::runtime_error("Can not spawn curl!");
	}
	try
	{
		while (fgets(buffer, sizeof(buffer), pipe) != NULL)
		{
			orResult += buffer;
		}
	}
	catch(...)
	{
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	std::stringstream ss(orResult);
	string token;

	while (getline(ss, token, '\"'))
	{
		std::size_t found = token.find("https");
		token = token.substr(9, token.length());
		if (found != std::string::npos)
		{
			std::stringstream ssb(token);
			string tokenb;
			while (getline(ssb, tokenb, '/'))
			{
				std::size_t foundb = tokenb.find("v");
				if (foundb != std::string::npos)
				{
					return tokenb.substr(1, tokenb.length());
				}
			}
		}
	}
	return "";
}

void getSolc(string version)
{
	string wgetCmd = "wget https://github.com/ethereum/solidity/releases/download/v" + version + "/solc-static-linux";
	system(wgetCmd.c_str());
}

void delOldSolc()
{
	string rmCmd = "rm -rf ./solc";
	system(rmCmd.c_str());
}

void moveSolc()
{
	string mvCmd = "mv solc-static-linux solc";
	system(mvCmd.c_str());
	string chCmd = "chmod 775 ./solc";
	system(chCmd.c_str());
}

string getEpiVer()
{
	string orCmd = "curl https://internal.services.web3labs.com/api/epirus/versions/latest";
	string orResult = "";
	char buffer[128];
	FILE* pipe = popen(orCmd.c_str(), "r");
	if (!pipe)
	{
		throw std::runtime_error("Can not spawn curl!");
	}
	try
	{
		while (fgets(buffer, sizeof(buffer), pipe) != NULL)
		{
			orResult += buffer;
		}
	}
	catch (...)
	{
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return orResult;
}

void getEpiSrc(string version)
{
	string fileName = "epirus-cli-shadow-" + version + ".tar";
	string wgetCmd = "wget https://github.com/epirus-io/epirus-cli/releases/download/v" + version + "/" + fileName;
	system(wgetCmd.c_str());
}

void crEpiTmp(string version)
{
	string fileName = "epirus-cli-shadow-" + version + ".tar";
	string unZipCmd = "tar -xvf ./" + fileName;
	system(unZipCmd.c_str());
}

void moveEpi(string version)
{
	string fileName = "epirus-cli-shadow-" + version;
	string clearEpiCmd = "rm -rf ./epirus-cli";
	string rmSymbLink = "rm -rf ./epirus";
	system(clearEpiCmd.c_str());
	system(rmSymbLink.c_str());
	string moveCmd = "mv " + fileName + " epirus-cli";
	system(moveCmd.c_str());
	string fileName2 = "epirus-cli-shadow-" + version + "-SNAPSHOT";
	string moveCmd2 = "mv " + fileName2 + " epirus-cli";
	system(moveCmd2.c_str());
	string crSymbLink = "ln -s ./epirus-cli/bin/epirus ./epirus";
	system(crSymbLink.c_str());

}

void delEpiTmp(string version)
{
	string fileName = "epirus-cli-shadow-" + version + ".tar";
	string rmCmd = "rm -rf ./" + fileName;
	system(rmCmd.c_str());
}

string getEthVer()
{
	string orCmd = "curl https://github.com/ethereum/go-ethereum/releases/latest";
	string orResult = "";
	char buffer[128];

	FILE* pipe = popen(orCmd.c_str(), "r");
	if (!pipe)
	{
		throw std::runtime_error("Can not spawn curl!");
	}
	try
	{
		while (fgets(buffer, sizeof(buffer), pipe) != NULL)
		{
			orResult += buffer;
		}
	}
	catch (...)
	{
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	std::istringstream ss(orResult);
	string token;

	while (getline(ss, token, '\"'))
	{
		std::size_t found = token.find("https");
		token = token.substr(9, token.length());
		if (found!=std::string::npos)
		{
			//cout << "Found !";
			std::istringstream ssb(token);
			string tokenb;
			while (getline(ssb, tokenb, '/'))
			{
				//cout << tokenb;
				std::size_t foundb = tokenb.find("v");
				if (foundb!=std::string::npos)
				{
					return tokenb.substr(1, tokenb.length());
				}
			}
		}
	}
	return "";

}

void getEthSrc(string version)
{
	string fileName = "v" + version + ".zip";
	string wgetCmd = "wget https://github.com/ethereum/go-ethereum/archive/" + fileName;
	system(wgetCmd.c_str());
}

void clearOldEth()
{
	string fileName[] = {"abidump", "abigen", "bootnode", "checkpoint-admin",
	"clef", "devp2p", "ethkey", "evm", "faucet", "geth", "p2psim", "puppeth", "rlpdump"};
	for (int i = 0; i < sizeof(fileName)/sizeof(*fileName) ; i++)
	{
		//cout << fileName[i] << "\n";
		string rmCmd = "rm -rf ./" + fileName[i];
		system(rmCmd.c_str());
	}
}

void unTmp(string version)
{
	string fileName = "v" + version + ".zip";
	string unzipCmd = "unzip " + fileName;
	system(unzipCmd.c_str());
}

void compileEth(string version)
{
	string makeCompile = "make -C ./go-ethereum-" + version + " all";
	system(makeCompile.c_str());
}

void moveEth(string version)
{
	string moveExecutable = "mv ./go-ethereum-" + version + "/build/bin/* ./";
	system(moveExecutable.c_str());
}

void delTmp(string version)
{
	string delCmd = "rm -rf ./go-ethereum-" + version;
	system(delCmd.c_str());
	string rmZip = "rm -rf ./v" + version + ".zip";
	system(rmZip.c_str());
}
