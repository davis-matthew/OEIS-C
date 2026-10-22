/*
 * A387462  Composite cyclic numbers that are of the form 4n-1.
 * https://oeis.org/A387462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th composite integer k ≡ 3 (mod 4) such that
 *            gcd(k,φ(k)) = 1 (i.e., a composite cyclic number of the form
 *            4n‑1)
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   calc_phi, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* Last integer checked during generation */
    static uint64_t last_checked = 14;   /* 15 will be first candidate */

    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */

    /* Generate terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        ++last_checked;                   /* next candidate */

        /* Only numbers congruent to 3 mod 4 are possible */
        if ((last_checked & 3ULL) == 3ULL) {
            seq_t val  = (seq_t)last_checked;
            seq_t phi  = calc_phi(val);     /* φ(val) */
            /* val composite iff φ < val‑1; cyclic iff gcd(val,phi)=1 */
            if (gcd(val, phi) == (seq_t)1 && phi < val - (seq_t)1) {
                /* Ensure cache has space */
                if (cache_len >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap << 1 : 4;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  cache_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;  /* allocation failed */
                    cache = tmp;
                }
                cache[cache_len++] = val;
            }
        }
    }
    return cache[(size_t)(n - 1)];
}
