# Crestron-NodeRed
This is a NodeRed Plugin to Crestron Prefessor(Use Tcp Socket to commucation)

The Plugin has 3 types,there is

**Digital**

Accept Value is true/false or 0/1

**Analog**

Accept Value is 0-65535

**Serial**

Accept Value is any String(length is depend crestron marco)

**技术交流群：107927710，里面有很多大神呦**



# How to use
1. install nodejs and npm in your computer
2. install node-red `sudo npm install -g node-red`
3. install crestron-node `sudo npm install -g crestron-node`
4. run node-red
5. use crestron-node
   ![crestron-node](images/crestron-node.png)
6. config Crestron Professor Server 
   ![crestron-node-server](images/server.png)
7. Fill in the ID and choose the data type,Make sure to match the pins of Crestron's macro file
   ![crestron-node-edit](images/edit.png)
8. send the msg to the node
9. Open Crestron SIMPL Software to edit CresNode 
   ![crestron-node-edit](images/simpl.png)



# Test
I created 50 nodes and sent 50 messages to the Crestron host every 1s, 
which is equivalent to a message of 20ms. After a long test, the information was sent steadily

![crestron-node](images/node-test.png)

