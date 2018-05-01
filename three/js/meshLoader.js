function loadMeshData(string)
{
  var lines = string.split("\n");
  var positions = [];
  var normals = [];
  var faces = [];

  numVertices = 0;
  for ( var i = 0 ; i < lines.length ; i++ ) {
    var parts = lines[i].trimRight().split(' ');
    if ( parts.length > 0 ) {
      switch(parts[0]) {
        case 'v':  positions.push(new 
          THREE.Vector3(
            parseFloat(parts[1]),
            parseFloat(parts[2]),
            parseFloat(parts[3])
          ));
          numVertices = numVertices + 1;
          break;
        case 'vn':
          normals.push(new 
            THREE.Vector3(
              parseFloat(parts[1]),
              parseFloat(parts[2]),
              parseFloat(parts[3])));
          break;
        case 'f':
          var f1 = parts[1].split('/');
          var f2 = parts[2].split('/');
          var f3 = parts[3].split('/');
          // Array.prototype.push.apply(
          //   vertices, positions[parseInt(f1[0]) - 1]);
          // Array.prototype.push.apply(
          //   vertices, normals[parseInt(f1[2]) - 1]);
          // Array.prototype.push.apply(
          //   vertices, positions[parseInt(f2[0]) - 1]);
          // Array.prototype.push.apply(
          //   vertices, normals[parseInt(f2[2]) - 1]);
          // Array.prototype.push.apply(
          //   vertices, positions[parseInt(f3[0]) - 1]);
          // Array.prototype.push.apply(
          //   vertices, normals[parseInt(f3[2]) - 1]);
          f1 = parseInt(f1);
          f2 = parseInt(f2);
          f3 = parseInt(f3);

          faces.push(new THREE.Vector3(f1-1, f2-1, f3-1));

          break;
      }
    }
  }

  alert("Loaded mesh with " + numVertices + " vertices");
  
  // alert(faces.length());

  return {
    primitiveType: 'TRIANGLES',
    vertices: positions,
    vertexCount: numVertices,
    material: {ambient: 0.2, diffuse: 0.5, shininess: 10.0},
    faces: faces
  };
}

function loadMeshGeometry(data)
{
  var meshObject = loadMeshData(data);
  var geometry = new THREE.Geometry();

  geometry.vertices = meshObject.vertices;
  geometry.faces = meshObject.faces;

  // alert(geometry.vertices[0].isVector());

  geometry.computeBoundingSphere();
  var mesh = new THREE.Mesh(geometry, material);
  
  scene.add(mesh);
}

function loadMesh(filename) {
  $.ajax({
    url: filename,
    dataType: 'text'
  }).done(function(data) {
    loadMeshGeometry(data);
  }).fail(function() {
    alert('Failed to retrieve [' + filename + "]");
  });
}