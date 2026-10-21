/*
 * A259237  a(n) = least prime q such that q + prime(n) is a cube.
 * https://oeis.org/A259237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least prime q such that q + prime(n) is a cube
 * Range:     u8 none, u16 n <= 146, u32 n <= 3261, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, floor_cube_root, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/floor_cube_root.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (n == 0) return 0;           /* undefined for n=0, not used */

    /* Ensure cache has space for at least n elements */
    ensure_cache(n, &cache, &cap);

    /* Return cached value if already computed */
    if (cache[n - 1] != 0)
        return cache[n - 1];

    /* p = nth prime */
    uint32_t p = get_prime(n);

    /* Start with the smallest integer c such that c^3 > p */
    idx_t c = (idx_t)floor_cube_root((seq_t)p) + 1;

    seq_t q;
    for (;; ++c) {
        seq_t cube = (seq_t)c * (seq_t)c * (seq_t)c;
        q = cube - (seq_t)p;
        if (is_prime_big(q)) {
            cache[n - 1] = q;      /* memoize */
            return q;
        }
    }
}
