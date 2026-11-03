/*
 * A276970  Odd integers n such that 2^n == 2^9 (mod n).
 * https://oeis.org/A276970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth odd integer n satisfying 2^n == 512 (mod n). Computed
 *            by on-demand search using modular exponentiation
 * Range:     u8 n <= 16, u16 n <= 1044, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9939 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Return 1 if n is an odd integer with 2^n ≡ 512 (mod n).  */
static bool good_odd(uint64_t n)
{
    if (n % 2 == 0) return false;
    if (n <= 9) {
        /* Direct check, because exponent would be negative otherwise. */
        seq_t a = powmod((seq_t)2, n, (seq_t)n);
        seq_t b = (seq_t)(512 % n);
        return a == b;
    } else {
        /* n divides 2^(n-9)-1 */
        return powmod((seq_t)2, n - 9, (seq_t)n) == (seq_t)1;
    }
}

seq_t a(idx_t n)
{
    static seq_t *list      = NULL;
    static size_t   cap     = 0;
    static idx_t    len     = 0;
    static uint64_t cand    = 1;   /* first odd candidate */

    if (list == NULL) {
        cap  = 64;
        list = mem_new(seq_t, cap);
    }

    while (len < n) {
        if (good_odd(cand)) {
            if (len >= cap) {
                size_t newcap = cap * 2;
                seq_t *tmp = mem_resize(seq_t, list, newcap);
                if (!tmp) exit(1);
                list = tmp;
                cap  = newcap;
            }
            list[len++] = (seq_t)cand;
        }
        cand += 2;  /* next odd number */
    }

    return list[n - 1];
}
