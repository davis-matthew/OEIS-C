/*
 * A368120  Repeated application of the Syracuse map over F_2[x] starting from
 *          1+x^3, m = 1+x^2. Represented as the integers resulting from
 *          evaluating the polynomial at 2 over Z.
 * https://oeis.org/A368120
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368120:  a(-2) = 9,  a(n+1) = (a(n)>>1) XOR (a(n)<<1) if a(n) odd,
   else a(n)>>1.  The function works with any seq_t type and caches
   previously computed values for arbitrary order of calls. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* cache of computed terms, indexed by (n + 2) */
static seq_t *cache = NULL;

/* number of allocated elements */
static size_t cap = 0;

/* index of the last computed term (in array indices) */
static size_t last = SIZE_MAX;

static void ensure_capacity(size_t needed)
{
    if (cap >= needed) return;
    size_t new_cap = needed < 4 ? 4 : needed * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* out‑of‑memory */
    cache = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < -2) return (seq_t)0;  /* undefined, but safe */

    size_t idx = (size_t)(n + 2); /* map n to array index */

    /* First access: initialise cache and the first term a(-2)=9 */
    if (cache == NULL) {
        size_t init = idx + 1;
        if (init < 4) init = 4;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) abort();
        cap = init;
        cache[0] = (seq_t)9;      /* a(-2) */
        last = 0;
    }

    /* Expand if necessary */
    ensure_capacity(idx + 1);

    /* Compute missing terms up to idx */
    while (last < idx) {
        seq_t prev = cache[last];
        seq_t next;
        if ((prev & (seq_t)1) != 0)      /* odd */
            next = (prev >> 1) ^ (prev << 1);
        else
            next = prev >> 1;
        ++last;
        cache[last] = next;
    }

    return cache[idx];
}
