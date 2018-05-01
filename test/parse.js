String folder = "SOMEFOLDER";
String file = "filename.obj";
 
ArrayList<PVector> srf = new ArrayList<PVector>();
ArrayList<PVector> norm = new ArrayList<PVector>();
ArrayList<int[]> faces = new ArrayList<int[]>();
 
void setup() {
  noLoop();
}
 
void draw() {
  loadSrf(folder+file);
  writeSrf(folder+file.substring(0,file.length()-3)+"vbo");
  exit();
}
 
void loadSrf(String name) {
  srf.clear();
  norm.clear();
  faces.clear();
  println("LOAD " + name);
  String[] lines = loadStrings(name);
  for(int i=0;i<lines.length;++i) {
   if(lines[i].length() > 2) {
      //vertices
      if (lines[i].substring(0,2).equals("v ")) {
        float info[] = float(split(lines[i], " "));
        srf.add(new PVector(info[1],info[2],info[3]));
      } 
      //normals
      else if(lines[i].substring(0,2).equals("vn")) {
        float info[] = float(split(lines[i], " "));
        norm.add(new PVector(info[1],info[2],info[3]));
      } 
      else if(lines[i].substring(0,2).equals("f ")) {
        String info[] = split(lines[i], " ");
        //sometimes obj's have different indices for different properties separated by '//'
        int info1[] = int(split(info[1],"//"));
        int info2[] = int(split(info[2],"//"));
        int info3[] = int(split(info[3],"//"));
        //obj has 1 based indexing and we need 0 based indexing
        faces.add(new int[] {info1[0]-1,info2[0]-1,info3[0]-1});
      }
    }
  }
}
 
//write the data in binary
void writeSrf(String name) {
  try {
    DataOutputStream dos = new DataOutputStream(new FileOutputStream(name));
    dos.writeInt(srf.size());
    dos.writeInt(faces.size());
    PVector v;
    for(int i=0;i<srf.size();++i) {
      v = srf.get(i);
      writeFloat(dos,v.x);
      writeFloat(dos,v.y);
      writeFloat(dos,v.z);
      v = norm.get(i);
      writeFloat(dos,v.x);
      writeFloat(dos,v.y);
      writeFloat(dos,v.z);
    }
    int[] f;
    for(int i=0;i<faces.size();++i) {
      f = faces.get(i);
      writeShort(dos,f[0]);
      writeShort(dos,f[1]);
      writeShort(dos,f[2]);
    }    
    dos.close();
  } catch(Exception e) {
  }
}
 
//write a float value little endian
void writeFloat(DataOutputStream dos, float f) throws IOException {
  int i = Float.floatToIntBits(f);
  dos.writeByte(i & 0xFF);
  dos.writeByte((i >> 8) & 0xFF);
  dos.writeByte((i >> 16) & 0xFF);
  dos.writeByte((i >> 24) & 0xFF);
}
 
//write a 16-bit integer little endian, integers larger than 65535 will get truncated
void writeShort(DataOutputStream dos, int i) throws IOException {
  dos.writeByte(i & 0xFF);
  dos.writeByte((i >> 8) & 0xFF);
}

//load mesh object
/*
    bytes 0-3 = number of vertices
    bytes 4-7 = number of faces
    vertex = 6x4 bytes position followed by normal
    faces = 3x2 bytes as unsigned 16 bit indices
*/
 
function loadVBO(url, vbo) {
    var xhr = new XMLHttpRequest();
 
    xhr.onreadystatechange = function () { 
        if (xhr.readyState == xhr.DONE) {
            if (xhr.status == 200 && xhr.response) {
                loadBuffers(xhr.response,vbo);
            } else {
                console.log("Failed to download:" + xhr.status + " " + xhr.statusText);
            }
        }
    } 
    // Open the request for the provided url 
    xhr.open("GET", url, true);
    // Set the responseType to 'arraybuffer' for ArrayBuffer response 
    xhr.responseType = "arraybuffer";    
    xhr.send();
}
 
//read ArrayBuffer into gl buffers
function loadBuffers(buffer, vbo) {
    var reader = new DataView(buffer);
        //get number of vertices and faces
    var numVertices = reader.getUint32(0);
    var numFaces = reader.getUint32(4);
    vbo.numVertices = numVertices;
    vbo.numFaces = numFaces;
    //put that data in some arrays
    vbo.vertexData = new Float32Array(buffer,8,numVertices*6);
    vbo.indexData = new Uint16Array(buffer, numVertices*24+8, numFaces*3);
    //push that data to the GPU
    vbo.vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo.vertexBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, vbo.vertexData, gl.STATIC_DRAW);
     
    vbo.indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, vbo.indexBuffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, vbo.indexData, gl.STATIC_DRAW);
}

function draw() {
//... some gl drawing stuff up here
 
  gl.bindBuffer(gl.ARRAY_BUFFER, vbo.vertexBuffer);
  gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, 3, gl.FLOAT, false,24,0);
  gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute, 3, gl.FLOAT, false,24,12);
 
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, vbo.indexBuffer);
  gl.drawElements(gl.TRIANGLES, vbo.numFaces*3, gl.UNSIGNED_SHORT, 0);
}