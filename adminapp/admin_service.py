from jsonsocket import Client, Server

host = '127.0.0.1'
port = '8888'

client = Client()

message = raw_input("please enter message: ")
print "you entered", message

##client.connect(host, port).send({'some_list':[123,456]})
client.connect(host, port).send(message)
response = client.recv()

client.close()
