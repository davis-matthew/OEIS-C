/*
 * A246201  Permutation of natural numbers: a(1) = 1, a(A014580(n)) =
 *          (2*a(n))+1, a(A091242(n)) = 2*a(n), where A014580(n) = binary code
 *          for n-th irreducible polynomial over GF(2), A091242(n) = binary
 *          code for n-th reducible polynomial over GF(2).
 * https://oeis.org/A246201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n > 1: if n is the binary code of an irreducible
 *            polynomial over GF(2), a(n) = 1 + 2*a( A091226(n) ), else a(n) =
 *            2*a( A091245(n) ); (A091226 and A091245 give the ordinal
 *            positions of the irreducible and reducible polynomials in the
 *            binary order.)
 * Range:     u8 n <= 42, u16 n <= 241, u32 n <= 2200, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static inline int poly_degree(uint32_t p) {
    return 31 - __builtin_clz(p);
}

static inline uint32_t poly_mod(uint32_t f, uint32_t g) {
    int dg = poly_degree(g);
    while (f != 0) {
        int df = poly_degree(f);
        if (df < dg) break;
        f ^= g << (df - dg);
    }
    return f;
}

/* cache for sequence values */
static seq_t *cache        = NULL;
static size_t cache_len    = 0;
static size_t cache_cap    = 0;

/* list of irreducible polynomials found so far */
static uint32_t *irred_list = NULL;
static size_t irred_len     = 0;
static size_t irred_cap     = 0;

/* counters of polynomials */
static size_t irred_count = 0;
static size_t red_count   = 0;

/* ensure cache has values up to idx (inclusive) */
static void ensure_cache(idx_t idx) {
    if ((idx + 1U) <= cache_len) return;

    size_t need = (size_t)(idx + 1U);
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need + 16;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    if (irred_list == NULL) {
        irred_cap   = 16;
        irred_list  = (uint32_t*)malloc(irred_cap * sizeof(uint32_t));
    }

    /* initialise base case if first time */
    if (cache_len == 0) {
        cache[0] = 0;          // unused
        cache[1] = 1;          // a(1)=1
        cache_len = 2;
    }

    for (size_t i = cache_len; i < need; ++i) {
        uint32_t val = (uint32_t)i;   // polynomial code represented by integer i
        bool irreducible = true;

        if (val != 0) {
            int deg = poly_degree(val);
            int max_div_deg = deg >> 1;
            for (size_t j = 0; j < irred_len; ++j) {
                uint32_t q = irred_list[j];
                int deg_q = poly_degree(q);
                if (deg_q == 0) continue;             // skip constant polynomial
                if (deg_q > max_div_deg) break;
                if (poly_mod(val, q) == 0) {
                    irreducible = false;
                    break;
                }
            }
        }

        size_t k;
        seq_t ak;
        if (irreducible && val >= 2U) {          /* exclude the constant polynomial */
            ++irred_count;
            k = irred_count;                      // index among irreducible polynomials
            ak = cache[k];
            ak = ak << 1;
            ak += (seq_t)1;
            cache[i] = ak;

            /* store this polynomial for future divisor checks */
            if (irred_len == irred_cap) {
                irred_cap = irred_cap * 2 + 16;
                irred_list = (uint32_t*)realloc(irred_list,
                                 irred_cap * sizeof(uint32_t));
            }
            irred_list[irred_len++] = val;
        } else {
            ++red_count;
            k = red_count;                        // index among reducible polynomials
            ak = cache[k];
            ak = ak << 1;
            cache[i] = ak;
        }
    }
    cache_len = need;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety: undefined for n<1 */
    ensure_cache(n);
    return cache[n];
}
