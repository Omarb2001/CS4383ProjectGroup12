 #version 330 core
 in vec3 N;
 in vec3 L;
 in vec3 E;
 in vec3 H;
 in vec4 eyePosition;
 

uniform vec4 lightPosition;
uniform mat4 Projection;
uniform mat4 ModelView;

 
 uniform vec4 lightDiffuse;
 uniform vec4 lightSpecular; 
 uniform vec4 lightAmbient;
 uniform vec4 surfaceDiffuse;
 uniform vec4 surfaceSpecular;
 uniform float shininess;
 uniform vec4 surfaceAmbient;
 uniform vec4  surfaceEmissive;


 uniform vec4 spotlightPosition;
 uniform vec4 spotlightDirection;
 uniform vec4 spotlightDiffuse;
 uniform vec4 spotlightSpecular; 
 uniform vec4 spotlightAmbient;
 uniform float cuttoffAngle;


void main()
{
     vec3 Normal = normalize(N);
     vec3 Light  = normalize(lightPosition - eyePosition).xyz;
     vec3 Eye    = normalize(E);
     vec3 Half   = normalize(H);
	
    float Kd = max(dot(Normal, Light), 0.0);
    float Ks = pow(max(dot(reflect(-Light, Normal),Eye), 0.0), shininess);
    float Ka = 1.0;
    vec4 diffuse  = Kd * lightDiffuse*surfaceDiffuse;
    vec4 specular = Ks * lightSpecular*surfaceSpecular;
    vec4 ambient  = Ka * lightAmbient*surfaceAmbient;


    vec4 spotLight  = normalize(spotlightPosition - eyePosition); 
    float sKd = max(dot(Normal, spotLight.xyz), 0.0);
    float sKs = pow(max(dot(reflect(-spotLight.xyz, Normal),Eye), 0.0), shininess);
    float sKa = 0.0f;
    float angle = dot(spotLight, spotlightDirection);
    float ca = radians(cuttoffAngle);
    float spot; 
        
   
   if(acos(angle) < ca){
     spot = (max(angle,0));
   }
   else{
      spot = 0;
   }


    vec4 spotdiffuse  = sKd * spotlightDiffuse*surfaceDiffuse;
    vec4 spotspecular = sKs * spotlightSpecular*surfaceSpecular;
    vec4 spotambient  = sKa * spotlightAmbient*surfaceAmbient;
  

    gl_FragColor = (spot * (surfaceEmissive + spotambient + ( spotdiffuse + spotspecular))) +  ( surfaceEmissive + ambient + ( diffuse + specular));
}
