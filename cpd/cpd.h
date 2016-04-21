/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_h
#define cpd_h

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#ifdef _WIN32
#ifdef PD_INTERNAL
#define CPD_EXTERN __declspec(dllexport) extern
#else
#define CPD_EXTERN __declspec(dllimport) extern
#endif
#else
#define CPD_EXTERN extern
#endif

#if defined(_MSC_VER) && !defined(_LANGUAGE_C_PLUS_PLUS) && !defined(__cplusplus)
#define CPD_EXTERN_STRUCT extern struct
#else
#define CPD_EXTERN_STRUCT struct
#endif

CPD_EXTERN_STRUCT _symbol;
CPD_EXTERN_STRUCT _symbol;
CPD_EXTERN_STRUCT _binbuf;
CPD_EXTERN_STRUCT _internal;
CPD_EXTERN_STRUCT _gpointer;
CPD_EXTERN_STRUCT _glist;
CPD_EXTERN_STRUCT _text;
CPD_EXTERN_STRUCT _iemgui;
CPD_EXTERN_STRUCT _atom;

//! @defgroup cpd cpd
//! @brief The low level c interface of zpd.
//! @details The cpd interface is

//! @addtogroup cpd
//! @{

//! @brief The type used for samples during digital signal processing.
typedef float cpd_sample;
//! @brief The type used for floating point numbers.
typedef float cpd_float;
//! @brief The opaque type used to bind and unbind cpd_instance with string characters.
typedef struct _symbol      cpd_tie;
//! @brief The opaque type used for fast comparaison of string characters.
typedef struct _symbol      cpd_symbol;
//! @brief The opaque type used for internal messages.
typedef struct _list        cpd_list;
//! @brief The opaque type used for internal instance management.
typedef struct _internal    cpd_internal;
//! @brief The opaque type used for graphical pointer (not implemented).
typedef struct _gpointer    cpd_gpointer;
//! @brief The opaque type used for a patch.
typedef struct _glist       cpd_patch;
//! @brief The opaque type used for an object.
typedef struct _text        cpd_object;
//! @brief The opaque type used for a iem gui object.
typedef struct _iemgui      cpd_gui;
//! @brief The instance is the main interface to communicate within the cpd environment
//! @details The instance manages the posts to the console, the midi events, the messages
//! and the digital signal processing. It is also the interface to load and delete patches.
typedef struct _instance
{
    cpd_internal* cpd_internal_ptr;
}cpd_instance;

//! @brief The type of messages in a list.
typedef enum
{
    CPD_NULL,     //!< @brief The element is undefined or null.
    CPD_FLOAT,    //!< @brief The element is a floating point number.
    CPD_SYMBOL,   //!< @brief The element is a symbol.
    CPD_POINTER   //!< @brief The element is a graphical pointer.
} cpd_atomtype;

//! @brief The available types of gui object.
typedef enum
{
    CPD_GUI_BANG          = 0, //!< @brief The gui is a bang.
    CPD_GUI_SLIDERH       = 1, //!< @brief The gui is an horizontal slider.
    CPD_GUI_SLIDERV       = 2, //!< @brief The gui is a vertical slider.
    CPD_GUI_TOGGLE        = 3, //!< @brief The gui is a toggle.
    CPD_GUI_NUMBER        = 4, //!< @brief The gui is a number box.
    CPD_GUI_RADIOH        = 5, //!< @brief The gui is an horizontal radio.
    CPD_GUI_RADIOV        = 6, //!< @brief The gui is an horizontal radio.
    CPD_GUI_VUMETER       = 7, //!< @brief The gui is a VU-meter.
    CPD_GUI_PANEL         = 8  //!< @brief The gui is a panel.
} cpd_guitype;

//! @brief The bang method prototype.
typedef void (*cpd_hook_bang)(struct _instance* instance, cpd_tie* tie);
//! @brief The float method prototype.
typedef void (*cpd_hook_float)(struct _instance* instance, cpd_tie* tie, cpd_float f);
//! @brief The symbol method prototype.
typedef void (*cpd_hook_symbol)(struct _instance* instance, cpd_tie* tie, cpd_symbol* s);
//! @brief The gpointer method prototype.
typedef void (*cpd_hook_gpointer)(struct _instance* instance, cpd_tie* tie, cpd_gpointer *gp);
//! @brief The list method prototype.
typedef void (*cpd_hook_list)(struct _instance* instance, cpd_tie* tie, cpd_list *list);
//! @brief The anything method prototype.
typedef void (*cpd_hook_anything)(struct _instance* instance, cpd_tie* tie, cpd_symbol *s, cpd_list *list);
//! @brief The method prototype to posts from the console.
typedef void (*cpd_hook_post)(struct _instance* instance, const char *s);
//! @brief The midi note on method prototype.
typedef void (*cpd_hook_noteon)(struct _instance* instance, int port, int channel, int pitch, int velocity);
//! @brief The midi control change method prototype.
typedef void (*cpd_hook_controlchange)(struct _instance* instance, int port, int channel, int control, int value);
//! @brief The midi progam change method prototype.
typedef void (*cpd_hook_programchange)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi pitch bend method prototype.
typedef void (*cpd_hook_pitchbend)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi after touch method prototype.
typedef void (*cpd_hook_aftertouch)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi poly after touch method prototype.
typedef void (*cpd_hook_polyaftertouch)(struct _instance* instance, int port, int channel, int pitch, int value);
//! @brief The midi byte method prototype.
typedef void (*cpd_hook_byte)(struct _instance* instance, int port, int value);

//! @brief The console methods.
typedef struct _hook_console
{
    cpd_hook_post  m_log;    //!< @brief The log post method
    cpd_hook_post  m_normal; //!< @brief The normal post method
    cpd_hook_post  m_error;  //!< @brief The error post method
    cpd_hook_post  m_fatal;  //!< @brief The fatal post method
}cpd_hook_console;

//! @brief The midi methods.
typedef struct _hook_midi
{
    cpd_hook_noteon           m_noteon;           //!< brief The note on method
    cpd_hook_controlchange    m_controlchange;    //!< brief The control change method
    cpd_hook_programchange    m_programchange;    //!< brief The program change method
    cpd_hook_pitchbend        m_pitchbend;        //!< brief The pitch bend method
    cpd_hook_aftertouch       m_aftertouch;       //!< brief The after touch method
    cpd_hook_polyaftertouch   m_polyaftertouch;   //!< brief The poly after touch method
    cpd_hook_byte             m_byte;             //!< brief The byte method
}cpd_hook_midi;

//! @brief The messages methods.
typedef struct _hook_message
{
    cpd_hook_bang     m_bang;     //!< @brief The bang method
    cpd_hook_float    m_float;    //!< @brief The float method
    cpd_hook_symbol   m_symbol;   //!< @brief The symbol method
    cpd_hook_gpointer m_gpointer; //!< @brief The gpointer method
    cpd_hook_list     m_list;     //!< @brief The list method
    cpd_hook_anything m_anything; //!< @brief The anything method
}cpd_hook_message;

//! @defgroup env environment
//! @brief The global environment of cpd.

//! @addtogroup env
//! @{

//! @brief Initializes the Pure Data environment.
//! @details The method should be called only one time at the initialization, before
//! everything else.
CPD_EXTERN void cpd_init();

//! @brief Clears the Pure Data environment.
//! @details The method should be called only one time when the Pure Data environment will
//! never be called again.
CPD_EXTERN void cpd_clear();




//! @brief Gets the major version of Pure Data.
CPD_EXTERN unsigned int cpd_version_getmajor();

//! @brief Gets the minor version of Pure Data.
CPD_EXTERN unsigned int cpd_version_getminor();

//! @brief Gets the bug version of Pure Data.
CPD_EXTERN unsigned int cpd_version_getbug();




//! @brief Clears the search path of Pure Data.
//! @details Clears the search path for all the environment. The path will be used when you
//! try to open a patch, to load an abstraction or an external.
CPD_EXTERN void cpd_searchpath_clear();

//! @brief Adds a path to the search path of Pure Data.
//! @details Adds a path to the search path for all the environment. The path will be used
//! when you try to open a patch, to load an abstraction or an external.
//! @param path The path to add.
CPD_EXTERN void cpd_searchpath_add(const char* path);


//! @}

//! @defgroup instance instance
//! @brief The instance of cpd.

//! @addtogroup instance
//! @{


//! @brief Creates a new instance.
//! @details If you want to implement your own instance, the first memeber of the structure
//! must be a cpd_instance, after this you can add everything you want. You should allocate
//! your instance with this function using sizeof(myinstance).
//! @code{.c}
//! struct myinstance
//! {
//!     cpd_instance instance;
//!     int* values;
//! };
//! ...
//! myinstance* inst = cpd_instance_new(sizeof(myinstance));
//! if(inst)
//!     inst->value = (float *)malloc(512 * sizeof(float));
//! ...
//! @endcode
//! @param size The size of memory to allocate in bytes.
//! @return A pointer to the initialized cpd_instance or NULL if the allocation failed.
CPD_EXTERN cpd_instance* cpd_instance_new(size_t size);

//! @brief Deletes an instance.
//! @details You must first delete the members of your instance if need and therefater call
//! this method to free the instance.
//! @code{.c}
//! struct myinstance
//! {
//!     cpd_instance instance;
//!     int* values;
//! };
//! ...
//! myinstance* inst;
//! ..
//! free(inst->value);
//! cpd_instance_free(inst);
//! ...
//! @endcode
//! @param instance The pointer to the instance.
CPD_EXTERN void cpd_instance_free(cpd_instance* instance);

//! @brief Sets the current instance.
CPD_EXTERN void cpd_instance_set(cpd_instance* instance);

//! @brief Sets the print methods of an instance.
//! @param instance The instance.
//! @param consolehook The set of post methods.
CPD_EXTERN void cpd_instance_set_hook_console(cpd_instance* instance, cpd_hook_console* consolehook);

//! @brief Sets the midi methods of an instance.
//! @param instance The instance.
//! @param midihook The set of midi methods.
CPD_EXTERN void cpd_instance_set_hook_midi(cpd_instance* instance, cpd_hook_midi* midihook);

//! @brief Binds an instance to a tie.
//! @param instance The instance.
//! @param tie The tie to bind to.
//! @param messagehook The set of message methods.
CPD_EXTERN void cpd_instance_bind(cpd_instance* instance, cpd_tie* tie, cpd_hook_message* messagehook);

//! @brief Unbinds an instance to a tie.
//! @param instance The instance.
//! @param tie The tie to unbind from.
CPD_EXTERN void cpd_instance_unbind(cpd_instance* instance, cpd_tie* tie);

//! @brief Prepares the digital signal processing for an instance.
//! @param instance The instance.
//! @param nins The number of inputs.
//! @param nouts The number of outputs.
//! @param samplerate The sample rate.
//! @param nsamples The number of samples per ticks.
CPD_EXTERN void cpd_instance_dsp_prepare(cpd_instance* instance, const int nins, const int nouts, const int samplerate, const int nsamples);

//! @brief Performs the digital signal processing for an instance.
//! @param instance The instance.
//! @param nsamples The number of samples.
//! @param nins The number of inputs.
//! @param inputs The input samples matrix.
//! @param nouts The number of outputs.
//! @param outputs The output samples matrix.
CPD_EXTERN void cpd_instance_dsp_perform(cpd_instance* instance, int nsamples, const int nins, const cpd_sample** inputs, const int nouts, cpd_sample** outputs);

//! @brief Releases the digital signal processing for an instance.
//! @param instance The instance.
CPD_EXTERN void cpd_instance_dsp_release(cpd_instance* instance);

//! @brief Gets the sample rate of an instance.
//! @param instance The instance.
//! @return The current sample rate of the instance.
CPD_EXTERN int cpd_instance_get_samplerate(cpd_instance* instance);

//! @brief Sends a normal post to the current instance.
//! @param instance The instance.
//! @param message The message in the printf format.
CPD_EXTERN void cpd_instance_post_normal(cpd_instance* instance, char const* message, ...);

//! @brief Sends a log post to the current instance.
//! @param instance The instance.
//! @param message The message in the printf format.
CPD_EXTERN void cpd_instance_post_log(cpd_instance* instance, char const* message, ...);

//! @brief Sends an error to the current instance.
//! @param instance The instance.
//! @param message The message in the printf format.
CPD_EXTERN void cpd_instance_post_error(cpd_instance* instance, char const* message, ...);

//! @brief Sends a fatal error to the current instance.
//! @param instance The instance.
//! @param message The message in the printf format.
CPD_EXTERN void cpd_instance_post_fatal(cpd_instance* instance, char const* message, ...);

//! @brief Sends a bang through a tie.
//! @param instance The instance.
//! @param tie The tie.
CPD_EXTERN void cpd_instance_send_bang(cpd_instance* instance, cpd_tie const* tie);

//! @brief Sends a float through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param value The float value.
CPD_EXTERN void cpd_instance_send_float(cpd_instance* instance, cpd_tie const* tie, cpd_float value);

//! @brief Sends a gpointer through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param pointer The gpointer.
CPD_EXTERN void cpd_instance_send_gpointer(cpd_instance* instance, cpd_tie const* tie, cpd_gpointer const* pointer);

//! @brief Sends a symbol through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param symbol The symbol.
CPD_EXTERN void cpd_instance_send_symbol(cpd_instance* instance, cpd_tie const* tie, cpd_symbol const* symbol);

//! @brief Sends a list through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param list The list.
CPD_EXTERN void cpd_instance_send_list(cpd_instance* instance, cpd_tie const* tie, cpd_list const* list);

//! @brief Sends anything through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param selector The selector.
//! @param list The list.
CPD_EXTERN void cpd_instance_send_anything(cpd_instance* instance, cpd_tie const* tie, cpd_symbol const* selector, cpd_list const* list);

//! @brief Sends a midi note on event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param pitch The channel between 0 and 127.
//! @param velocity The velocity between 0 and 127.
CPD_EXTERN void cpd_instance_midi_noteon(cpd_instance* instance, int channel, int pitch, int velocity);

//! @brief Sends a midi control change event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param controller The controller between 0 and 127.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_controlchange(cpd_instance* instance, int channel, int controller, int value);

//! @brief Sends a midi program change event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_programchange(cpd_instance* instance, int channel, int value);

//! @brief Sends a midi pitch bend event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_pitchbend(cpd_instance* instance, int channel, int value);

//! @brief Sends a midi after touch event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_aftertouch(cpd_instance* instance, int channel, int value);

//! @brief Sends a midi poly after touch event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param pitch The channel between 0 and 127.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_polyaftertouch(cpd_instance* instance, int channel, int pitch, int value);

//! @brief Sends a midi byte event.
//! @param instance The instance.
//! @param port The port.
//! @param byte The byte value.
CPD_EXTERN void cpd_instance_midi_byte(cpd_instance* instance, int port, int byte);

//! @}






//! @brief Creates a new patch.
CPD_EXTERN cpd_patch* cpd_patch_new(const char* name, const char* path);

//! @brief Frees a patch.
CPD_EXTERN void cpd_patch_free(cpd_patch* patch);

//! @brief Gets the name of a patch.
CPD_EXTERN const char* cpd_patch_get_name(cpd_patch const* patch);

//! @brief Gets the path of a patch.
CPD_EXTERN const char* cpd_patch_get_path(cpd_patch const* patch);

//! @brief Gets the dollar zero of a patch.
CPD_EXTERN int cpd_patch_get_dollarzero(cpd_patch const* patch);

//! @brief Gets the x margin of a patch.
CPD_EXTERN int cpd_patch_get_x(cpd_patch const* patch);

//! @brief Gets the x margin of a patch.
CPD_EXTERN int cpd_patch_get_y(cpd_patch const* patch);

//! @brief Gets the width of a patch.
CPD_EXTERN int cpd_patch_get_width(cpd_patch const* patch);

//! @brief Gets the height of a patch.
CPD_EXTERN int cpd_patch_get_height(cpd_patch const* patch);

//! @brief Gets the first object of a patch.
CPD_EXTERN cpd_object* cpd_patch_get_first_object(cpd_patch const* patch);

//! @brief Gets the next object of a patch.
CPD_EXTERN cpd_object* cpd_patch_get_next_object(cpd_patch const* patch, cpd_object const* previous);




//! @brief Gets the name of an object.
CPD_EXTERN cpd_symbol* cpd_object_get_name(cpd_object const* object);

//! @brief Gets the name of an object.
CPD_EXTERN void cpd_object_get_text(cpd_object const* object, int* size, char** text);

CPD_EXTERN void cpd_object_get_bounds(cpd_object const* object, cpd_patch const* patch, int* x, int* y, int* width, int* height);

//! @brief Gets the x position of an object.
CPD_EXTERN int cpd_object_get_x(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the x position of an object.
CPD_EXTERN int cpd_object_get_y(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the width of an object.
CPD_EXTERN int cpd_object_get_width(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the height of an object.
CPD_EXTERN int cpd_object_get_height(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the if an object is a gui.
CPD_EXTERN char cpd_object_is_gui(cpd_object const* object);





//! @brief Gets the label of a gui.
CPD_EXTERN cpd_symbol* cpd_gui_get_label(cpd_gui const* gui);

//! @brief Gets the receive symbol of a gui.
CPD_EXTERN cpd_tie* cpd_gui_get_receive_tie(cpd_gui const* gui);

//! @brief Gets the send symbol of a gui.
CPD_EXTERN cpd_tie* cpd_gui_get_send_tie(cpd_gui const* gui);

//! @brief Gets the send symbol of a gui.
CPD_EXTERN cpd_guitype cpd_gui_get_type(cpd_gui const* gui);

//! @brief Gets the maximum value of a gui.
CPD_EXTERN float cpd_gui_get_maximum_value(cpd_gui const* gui);

//! @brief Gets the maximum value of a gui.
CPD_EXTERN float cpd_gui_get_minimum_value(cpd_gui const* gui);

//! @brief Gets the number of steps of a gui.
CPD_EXTERN int cpd_gui_get_number_of_steps(cpd_gui const* gui);

//! @brief Gets the number of steps of a gui.
CPD_EXTERN float cpd_gui_get_value(cpd_gui const* gui);

//! @brief Gets the x position of the label of a gui.
CPD_EXTERN int cpd_gui_get_label_x(cpd_gui const* gui, cpd_patch const* patch);

//! @brief Gets the y position of the label of a gui.
CPD_EXTERN int cpd_gui_get_label_y(cpd_gui const* gui, cpd_patch const* patch);






//! @brief Creates an opaque tie that can be understood by Pure Data.
CPD_EXTERN cpd_tie* cpd_tie_create(const char* name);

//! @brief Gets an opaque symbol that can be understood by Pure Data.
CPD_EXTERN char const* cpd_tie_get_name(cpd_tie const* tie);

//! @brief Creates an opaque symbol that can be understood by Pure Data.
CPD_EXTERN cpd_symbol* cpd_symbol_create(const char* symbol);

//! @brief Gets an opaque symbol that can be understood by Pure Data.
CPD_EXTERN char const* cpd_symbol_get_name(cpd_symbol const* symbol);



//! @brief Creates an opaque list that can be understood by Pure Data.
CPD_EXTERN cpd_list* cpd_list_create(size_t size);

//! @brief Clears a list.
CPD_EXTERN void cpd_list_free(cpd_list *list);

//! @brief Gets the size of a list.
CPD_EXTERN size_t cpd_list_get_size(cpd_list const* list);

//! @brief Gets the type of a data of the list.
CPD_EXTERN cpd_atomtype cpd_list_get_type(cpd_list const* list, size_t index);

//! @brief Gets the float value of a data of the list.
CPD_EXTERN cpd_float cpd_list_get_float(cpd_list const* list, size_t index);

//! @brief Gets the symbol of a data of the list.
CPD_EXTERN cpd_symbol* cpd_list_get_symbol(cpd_list const* list, size_t index);

//! @brief Gets the gpointer of a data of the list.
CPD_EXTERN cpd_gpointer* cpd_list_get_gpointer(cpd_list const* list, size_t index);

//! @brief Sets the float value of a data of the list.
CPD_EXTERN void cpd_list_set_float(cpd_list *list, size_t index, cpd_float value);

//! @brief Sets the symbol of a data of the list.
CPD_EXTERN void cpd_list_set_symbol(cpd_list *list, size_t index, cpd_symbol* symbol);

//! @brief Sets the gpointer of a data of the list.
CPD_EXTERN void cpd_list_set_gpointer(cpd_list *list, size_t index, cpd_gpointer* pointer);

CPD_EXTERN void* cpd_list_get_atom(cpd_list* list, size_t index);





//! @}


#undef CPD_EXTERN
#undef CPD_EXTERN_STRUCT


#endif /* cpd_h */
