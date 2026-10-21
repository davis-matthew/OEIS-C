/*
 * A298944  a(n) = 2^(c-1) mod c^2, where c is the n-th composite number.
 * https://oeis.org/A298944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(c-1) mod c^2, where c is the nth composite number
 * Range:     u8 n <= 8, u16 n <= 213, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, return 0 */

    /* cache of composite numbers */
    static idx_t *comp_arr = NULL;
    static size_t comp_cap = 0;
    static size_t comp_len = 0;
    static idx_t next = 4;               /* first composite */

    /* fill cache until we have n composites */
    while (comp_len < n) {
        if (!is_prime(next)) {
            if (comp_len == comp_cap) {
                comp_cap = comp_cap ? comp_cap << 1 : 16;
                comp_arr = (idx_t *)realloc(comp_arr, comp_cap * sizeof(idx_t));
            }
            comp_arr[comp_len++] = next;
        }
        ++next;
    }

    idx_t c = comp_arr[n - 1];           /* nth composite (1‑based) */
    seq_t mod = (seq_t)c * (seq_t)c;     /* c^2                */
    seq_t res = mod_pow((seq_t)2, (uint64_t)(c - 1), mod); /* 2^(c-1) mod c^2 */
    return res;
}
