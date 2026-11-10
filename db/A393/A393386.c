/*
 * A393386  Prime numbers in A391972, in the order in which they occur.
 * https://oeis.org/A393386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime among the terms of A391972, where A391972(1) =
 *            1, A391972(k) = 1 if A391972(k-1) is prime, else A391972(k-1) +
 *            (k - r), r = number of 1's among A391972(1..k-1). Computed by
 *            direct generation
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9824 (2026-11-10)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    static seq_t *pr = NULL;
    static size_t cnt = 0, cap = 0;
    static idx_t i = 1, ones = 1;
    static seq_t prev = 1;
    static bool prevprime = false;

    if (n == 0) return (seq_t)0;

    while ((idx_t)cnt < n) {
        i++;
        if (prevprime) {
            prev = (seq_t)1;
            ones++;
            prevprime = false;
        } else {
            prev = prev + (seq_t)(i - ones);
            prevprime = is_prime_u64((uint64_t)prev);
            if (prevprime) {
                if (cnt == cap) {
                    size_t nc = cap ? cap * 2 : 256;
                    seq_t *tmp = mem_resize(seq_t, pr, nc);
                    if (!tmp) abort();
                    pr = tmp;
                    cap = nc;
                }
                pr[cnt++] = prev;
            }
        }
    }
    return pr[(size_t)n - 1];
}
