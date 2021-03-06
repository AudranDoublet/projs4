#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../frog.h"

FrogObject *frogsdl_Init(FrogObject **args, size_t count, stack *p)
{
	UNUSED(args);
	UNUSED(count);
	UNUSED(p);

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		FrogErr_Post("SDLError", (char *) SDL_GetError());
		return NULL;
	}

	return FrogNone();
}

FrogObject *frogsdl_Quit(FrogObject **args, size_t count, stack *p)
{
	UNUSED(args);
	UNUSED(count);
	UNUSED(p);

	SDL_Quit();
	return FrogNone();
}

FrogObject *frogsdl_CreateTexture(FrogObject **args, size_t count, stack *p)
{
	UNUSED(count);
	UNUSED(p);

	for(int i = 0; i < 5; i++)
	{
		if(!FrogIsInt(args[i]))
		{
			FrogErr_TypeS("argument must be an integer");
			return NULL;
		}
	}

	SDL_Surface *s = SDL_CreateRGBSurface(SDL_SWSURFACE,
		FIValue(args[0]), FIValue(args[1]), 32, 0, 0, 0, 0);	

	SDL_SetSurfaceAlphaMod(s, 255);

	if(!s)
	{
		FrogErr_Post("SDLError", "Can't create surface");
		return NULL;
	}

	SDL_FillRect(s, NULL, SDL_MapRGB(s->format,
		FIValue(args[2]), FIValue(args[3]), FIValue(args[4])));

	return FromNativeInteger((long)s);
}

FrogObject *frogsdl_LoadTexture(FrogObject **args, size_t count, stack *p)
{
	UNUSED(count);
	UNUSED(p);

	FrogObject *o = Frog_ToString(args[0]);
	if(!o) return NULL;

	char *name = strtoutf8(o);
	if(!name) return NULL;

	SDL_Surface *s = IMG_Load(name);
	SDL_SetSurfaceAlphaMod(s, 255);

	if(!s)
	{
		FrogErr_Post("SDLErrror", "Can't load texture");
	}

	return FromNativeInteger((long) s);
}

FrogObject *frogsdl_FillTexture(FrogObject **args, size_t count, stack *p)
{
	UNUSED(count);
	UNUSED(p);

	for(int i = 0; i < 4; i++)
	{
		if(!FrogIsInt(args[i]))
		{
			FrogErr_TypeS("argument must be an integer");
			return NULL;
		}
	}

	SDL_Surface *s = (SDL_Surface *) FIValue(args[0]);

	SDL_FillRect(s, NULL, SDL_MapRGB(s->format,
		FIValue(args[1]), FIValue(args[2]), FIValue(args[3])));

	return FrogNone();
}

FrogObject *frogsdl_BlitTexture(FrogObject **args, size_t count, stack *p)
{
	UNUSED(count);
	UNUSED(p);

	for(int i = 0; i < 4; i++)
	{
		if(!FrogIsInt(args[i]))
		{
			FrogErr_TypeS("argument must be an integer");
			return NULL;
		}
	}

	SDL_Surface *s1 = (SDL_Surface *) FIValue(args[0]);
	SDL_Surface *s2 = (SDL_Surface *) FIValue(args[1]);

	SDL_Rect position;
	position.x = FIValue(args[2]);
	position.y = FIValue(args[3]);

	SDL_BlitSurface(s2, NULL, s1, &position);
	return FrogNone();
}

FrogObject *frogsdl_FlushWindow(FrogObject **args, size_t count, stack *p)
{
	UNUSED(count);
	UNUSED(p);

	for(int i = 0; i < 2; i++)
	{
		if(!FrogIsInt(args[i]))
		{
			FrogErr_TypeS("argument must be an integer");
			return NULL;
		}
	}

	SDL_Renderer *s1 = (SDL_Renderer *) FIValue(args[0]);
	SDL_Surface *s2 = (SDL_Surface *) FIValue(args[1]);

	SDL_Texture* s3 = SDL_CreateTextureFromSurface(s1, s2);

	SDL_RenderCopy(s1, s3, NULL, NULL);
	SDL_RenderPresent(s1);

	return FrogNone();
}

FrogObject *frogsdl_OpenWindow(FrogObject **args, size_t count, stack *p)
{
	UNUSED(count);
	UNUSED(p);

	FrogObject *str = Frog_ToString(args[0]);
	if(!str) return NULL;

	for(int i = 1; i < 3; i++)
	{
		if(!FrogIsInt(args[i]))
		{
			FrogErr_TypeS("argument must be an integer");
			return NULL;
		}
	}

	int width = FIValue(args[1]);
	int height = FIValue(args[2]);

	SDL_Window *s = SDL_CreateWindow(strtoutf8(str), 
                             SDL_WINDOWPOS_UNDEFINED, 
                             SDL_WINDOWPOS_UNDEFINED, 
                             width, height, 
                             0);

	if(!s)
	{
		FrogErr_Post("SDLError", (char *) SDL_GetError());
		return NULL;
	}

	SDL_Renderer *r = SDL_CreateRenderer(s, -1, 0);

	if(!r)
	{
		FrogErr_Post("SDLError", (char *) SDL_GetError());
		return NULL;
	}

	return FromNativeInteger((long) r);
}

FrogObject *sdlev_next(FrogObject *o)
{
	UNUSED(o);
	SDL_Event event;

	FrogObject **array = malloc(sizeof(FrogObject *) * 3);

	if(!array)
	{
		FrogErr_Memory();
		return NULL;
	}

		SDL_WaitEvent(&event);
	
	switch(event.type)
	{
	case SDL_QUIT:
		array[0] = FromNativeInteger(0);
		array[1] = FrogNone();
		array[2] = FrogNone();

		break;
	case SDL_MOUSEMOTION:
		array[0] = FromNativeInteger(1);
		array[1] = FromNativeInteger(event.motion.x);
		array[2] = FromNativeInteger(event.motion.y);

		break;
	case SDL_MOUSEBUTTONDOWN:
		array[0] = FromNativeInteger(2);
		array[1] = FromNativeInteger(event.button.x);
		array[2] = FromNativeInteger(event.button.y);

		break;
	case SDL_MOUSEBUTTONUP:
		array[0] = FromNativeInteger(3);
		array[1] = FromNativeInteger(event.button.x);
		array[2] = FromNativeInteger(event.button.y);

		break;
	default:
		array[0] = FromNativeInteger(-1);
		array[1] = FrogNone();
		array[2] = FrogNone();
		break;
	}

	return CreateTuple(array, 3);
}

FrogObject *sdlev_hasnext(FrogObject *o)
{
	UNUSED(o);
	return FrogTrue();
}

FrogObject *sdlev_init(FrogObject *o)
{
	UNUSED(o);
	return o;
}

FrogAsIterable sdlev_iter = { sdlev_init, sdlev_next, sdlev_hasnext };

FrogObject *frogsdl_WaitEvents(FrogObject **args, size_t count, stack *p)
{
	UNUSED(args);
	UNUSED(count);
	UNUSED(p);

	FrogIter *iter = (FrogIter *) CreateIterable0(NULL);
	if(!iter) return NULL;

	iter->iterable = &sdlev_iter;

	return (FrogObject *) iter;
}
