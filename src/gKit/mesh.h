
#ifndef _MESH_H
#define _MESH_H

#include <vector>

#define GLEW_NO_GLU
#include "GL/glew.h"

#include "vec.h"
#include "mat.h"
#include "color.h"

//! represenation d'un objet / maillage.
struct Mesh
{
    std::vector<vec3> positions;
    std::vector<vec2> texcoords;
    std::vector<vec3> normals;
    std::vector<vec3> colors;
    
    std::vector<unsigned int> indices;
    
    GLenum primitives;
    int count;
    GLuint vao;
    GLuint program;
};

//! construit un objet / maillage vide compose de primitives de type \param primitives : GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_LINES, etc.
Mesh create_mesh( const GLenum primitives );
//! detruit un objet.
void release_mesh( Mesh& m );

//! definit les coordonnees de texture du prochain sommet.
void vertex_texcoord( Mesh& m, const float u, const float v );
//! definit les coordonnees de texture du prochain sommet.
void vertex_texcoord( Mesh& m, const vec2& uv );
//! definit la normale du prochain sommet.
void vertex_normal( Mesh& m, const vec3& normal );
//! definit la normale du prochain sommet.
void vertex_normal( Mesh& m, const Vector& normal );
//! definit la couleur du prochain sommet.
void vertex_color( Mesh& m, const vec3& color );
//! definit la couleur du prochain sommet.
void vertex_color( Mesh& m, const Color& color );

//! insere un sommet et ses attirbuts dans l'objet. renvoie l'indice du sommet.
unsigned int push_vertex( Mesh& m, const vec3& position );
//! insere un sommet et ses attirbuts dans l'objet. renvoie l'indice du sommet.
unsigned int push_vertex( Mesh& m, const Point& position );

/*! insere un sommet et ses attributs dans l'objet. utilitaire, equivalent a:
\code
vertex_texcoord(m, u, v);
vertex_normal(m, normal);
push_vertex(m, position);
\endcode
*/
unsigned int push_vertex( Mesh& m, const vec3& position, const float u, const float v, const vec3& normal );
//! insere un sommet et ses attributs dans l'objet. utilitaire.
unsigned int push_vertex( Mesh& m, const Point& position, const float u, const float v, const Vector& normal );

/*! insere un sommet et ses attributs dans l'objet. utilitaire, equivalent a:
\code
vertex_texcoord(m, u, v);
push_vertex(m, position);
\endcode
*/
unsigned int push_vertex( Mesh& m, const vec3& position, const float u, const float v );
//! insere un sommet et ses attributs dans l'objet. utilitaire.
unsigned int push_vertex( Mesh& m, const Point& position, const float u, const float v );

/*! insere un sommet et ses attributs dans l'objet. utilitaire, equivalent a:
\code
vertex_normal(m, normal);
push_vertex(m, position);
\endcode
*/
unsigned int push_vertex( Mesh& m, const vec3& position, const vec3& normal );
//! insere un sommet et ses attributs dans l'objet. utilitaire.
unsigned int push_vertex( Mesh& m, const Point& position, const Vector& normal );

/*! insere un triangle dans l'objet, a, b, c sont les indices des sommets deja inseres dans l'objet. ne fonctionne pas avec les strips et les fans.
\code
Mesh m= create_mesh(GL_TRIANGLES);
unsigned int a= push_vertex(m, Point(ax, ay, az));
unsigned int b= push_vertex(m, Point(bx, by, bz));
unsigned int c= push_vertex(m, Point(cx, cy, cz));
push_triangle(m, a, b, c);
\endcode
*/
void push_triangle( Mesh& m, const unsigned int a, const unsigned int b, const unsigned int c );
void push_triangle_last( Mesh& m, const int a, const int b, const int c );

//! 
void restart_strip( Mesh& m );

//! renvoie min et max les coordonnees des extremites des positions des sommets de l'objet.
void bounds( const Mesh& m, Point& pmin, Point& pmax );

//! construit les buffers et le vertex array object necessaires pour dessiner l'objet avec openGL. utilitaire.
GLuint make_mesh_vertex_format( Mesh& m );
//! construit le shader program configure pour dessiner l'objet avec openGL. utilitaire.
GLuint make_mesh_program( Mesh& m );

//! dessine l'objet avec les transformations model, vue et projection.
void draw( Mesh& m, const Transform& model, const Transform& view, const Transform& projection );
//! dessine l'objet avec les transformations model, vue et projection. applique une texture a la surface de l'objet. ne fonctionne que si les coordonnees de textures sont fournies avec tous les sommets de l'objet. 
void draw( Mesh& m, const Transform& model, const Transform& view, const Transform& projection, GLuint texture );

#endif
