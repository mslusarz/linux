#ifndef __NOUVEAU_CRYPT_H__
#define __NOUVEAU_CRYPT_H__

#include <core/engine.h>
#include <core/engctx.h>

struct nouveau_crypt_chan {
	struct nouveau_engctx base;
};
INHERITS_NV_ENGCTX(nv_cryptch, struct nouveau_crypt_chan);

#define INHERITS_NV_CRYPT_CHAN(pfx, type) \
	NOUVEAU_UPCAST(pfx, cryptch, type, struct nouveau_crypt_chan) \
	INHERITS_NV_ENGCTX(pfx, type)

#define nouveau_crypt_context_create(p,e,c,g,s,a,f,d)                          \
	nouveau_engctx_create((p), (e), (c), (g), (s), (a), (f), (d))
#define nouveau_crypt_context_destroy(d)                                       \
	nouveau_engctx_destroy(&(d)->base)
#define nouveau_crypt_context_init(d)                                          \
	nouveau_engctx_init(&(d)->base)
#define nouveau_crypt_context_fini(d,s)                                        \
	nouveau_engctx_fini(&(d)->base, (s))

#define _nouveau_crypt_context_dtor _nouveau_engctx_dtor
#define _nouveau_crypt_context_init _nouveau_engctx_init
#define _nouveau_crypt_context_fini _nouveau_engctx_fini
#define _nouveau_crypt_context_rd32 _nouveau_engctx_rd32
#define _nouveau_crypt_context_wr32 _nouveau_engctx_wr32

struct nouveau_crypt {
	struct nouveau_engine base;
};
INHERITS_NV_ENGINE(nv_crypt, struct nouveau_crypt);

#define INHERITS_NV_CRYPT(pfx, type) \
	NOUVEAU_UPCAST(pfx, crypt, type, struct nouveau_crypt) \
	INHERITS_NV_ENGINE(pfx, type)

#define nouveau_crypt_create(p,e,c,d)                                          \
	nouveau_engine_create((p), (e), (c), true, "PCRYPT", "crypt", (d))
#define nouveau_crypt_destroy(d)                                               \
	nouveau_engine_destroy(&(d)->base)
#define nouveau_crypt_init(d)                                                  \
	nouveau_engine_init(&(d)->base)
#define nouveau_crypt_fini(d,s)                                                \
	nouveau_engine_fini(&(d)->base, (s))

#define _nouveau_crypt_dtor _nouveau_engine_dtor
#define _nouveau_crypt_init _nouveau_engine_init
#define _nouveau_crypt_fini _nouveau_engine_fini

extern struct nouveau_oclass nv84_crypt_oclass;
extern struct nouveau_oclass nv98_crypt_oclass;

#endif
