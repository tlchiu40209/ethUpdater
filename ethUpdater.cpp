#include <iostream>
#include <stdexcept>
#include <cstring>
#include <stdio.h>
#include <string>
#include <limits.h>
#include <unistd.h>
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
string getExePath();
string getExeLocation();

string programName = "";

int main(int argc, char **argv)
{
	string progNameLocal(argv[0]);
	if (progNameLocal.find("./") != std::string::npos)
	{
		progNameLocal = progNameLocal.substr(2, progNameLocal.length() -2);
	}
	programName = progNameLocal;
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
	string outputDir = getExeLocation();
	string wgetCmd = "wget -P " + outputDir + " https://github.com/ethereum/solidity/releases/download/v" + version + "/solc-static-linux";
	//cout << "getSolc " << wgetCmd << endl;
	system(wgetCmd.c_str());
}

void delOldSolc()
{
	string outputDir = getExeLocation();
	string rmCmd = "rm -rf " + outputDir + "solc";
	//cout << "delOldSolc " << rmCmd << endl;
	system(rmCmd.c_str());
}

void moveSolc()
{
	string outputDir = getExeLocation();
	string mvCmd = "mv " + outputDir + "solc-static-linux " + outputDir + "solc";
	//cout << "moveSolc " << mvCmd << endl;
	system(mvCmd.c_str());
	string chCmd = "chmod 775 " + outputDir + "solc";
	system(chCmd.c_str());
	//cout << "moveSolc " << chCmd << endl;
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
	string outputDir = getExeLocation();
	string fileName = "epirus-cli-shadow-" + version + ".tar";
	string wgetCmd = "wget -P " + outputDir + " https://github.com/epirus-io/epirus-cli/releases/download/v" + version + "/" + fileName;
	system(wgetCmd.c_str());
	//cout << "getEpiSrc " << wgetCmd << endl;
}

void crEpiTmp(string version)
{
	string outputDir = getExeLocation();
	string fileName = "epirus-cli-shadow-" + version + ".tar";
	string unZipCmd = "tar -xvf " + outputDir + fileName + " -C " + outputDir ;
	system(unZipCmd.c_str());
	//cout << "crEpiTmp " << unZipCmd << endl;
}

void moveEpi(string version)
{
	string outputDir = getExeLocation();
	string fileName = "epirus-cli-shadow-" + version;
	string clearEpiCmd = "rm -rf " + outputDir + "epirus-cli";
	string rmSymbLink = "rm -rf " + outputDir + "epirus";
	system(clearEpiCmd.c_str());
	system(rmSymbLink.c_str());
	//cout << "moveEpi " << clearEpiCmd << endl;
	//cout << "moveEpi " << rmSymbLink << endl;
	string moveCmd = "mv " + outputDir + fileName + " " + outputDir + "epirus-cli";
	system(moveCmd.c_str());
	//cout << "moveEpi " << moveCmd << endl;
	string fileName2 = "epirus-cli-shadow-" + version + "-SNAPSHOT";
	string moveCmd2 = "mv " + outputDir + fileName2 + " " + outputDir + "epirus-cli";
	system(moveCmd2.c_str());
	//cout << "moveEpi " << moveCmd2 << endl;
	string crSymbLink = "ln -s " + outputDir + "epirus-cli/bin/epirus " + outputDir + "epirus";
	system(crSymbLink.c_str());
	//cout << "moveEpi " << crSymbLink << endl;

}

void delEpiTmp(string version)
{
	string outputDir = getExeLocation();
	string fileName = "epirus-cli-shadow-" + version + ".tar";
	string rmCmd = "rm -rf " + outputDir + fileName;
	system(rmCmd.c_str());
	//cout << "delEpiTmp " << rmCmd << endl;
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
	string outputDir = getExeLocation();
	string fileName = "v" + version + ".zip";
	string wgetCmd = "wget -P " + outputDir + " https://github.com/ethereum/go-ethereum/archive/" + fileName;
	system(wgetCmd.c_str());
	//cout << "getEthSrc " << wgetCmd << endl;
}

void clearOldEth()
{
	string outputDir = getExeLocation();
	string fileName[] = {"abidump", "abigen", "bootnode", "checkpoint-admin",
	"clef", "devp2p", "ethkey", "evm", "faucet", "geth", "p2psim", "puppeth", "rlpdump"};
	for (int i = 0; i < sizeof(fileName)/sizeof(*fileName) ; i++)
	{
		string rmCmd = "rm -rf " + outputDir + fileName[i];
		system(rmCmd.c_str());
		//cout << "clearOldEth " << rmCmd << endl;
	}
}

void unTmp(string version)
{
	string outputDir = getExeLocation();
	string fileName = "v" + version + ".zip";
	string unzipCmd = "unzip " + outputDir + fileName + " -d " + outputDir;
	system(unzipCmd.c_str());
	//cout << "unTmp " << unzipCmd << endl;
}

void compileEth(string version)
{
	string outputDir = getExeLocation();
	string makeCompile = "make -C " + outputDir + "go-ethereum-" + version + " all";
	system(makeCompile.c_str());
	//cout << "compileEth " << makeCompile << endl;
}

void moveEth(string version)
{
	string outputDir = getExeLocation();
	string moveExecutable = "mv " + outputDir + "go-ethereum-" + version + "/build/bin/* " + outputDir;
	system(moveExecutable.c_str());
	//cout << "moveEth " << moveExecutable << endl;
}

void delTmp(string version)
{
	string outputDir = getExeLocation();
	string delCmd = "rm -rf " + outputDir + "go-ethereum-" + version;
	system(delCmd.c_str());
	//cout << "delTmp " << delCmd << endl;
	string rmZip = "rm -rf " + outputDir + "v" + version + ".zip";
	system(rmZip.c_str());
	//cout << "delTmp " << rmZip << endl;
}

string getExeLocation()
{
	int programNameSize = programName.length();
	return (getExePath()).substr(0, (getExePath()).length() - programNameSize);
}

string getExePath()
{
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	return std::string(result, (count > 0) ? count : 0);
}
