/*
 * A282191  Odd numbers divisible by 3 which are neither of the form p + 3^m +
 *          1 nor of the form p + 3^m - 1 with p prime.
 * https://oeis.org/A282191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation: odd multiples of 3 that are not of the
   form p + 3^m ± 1 for any prime p and integer m ≥ 0. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *seq_arr = NULL;   /* array of computed terms          */
static idx_t  seq_size = 0;     /* number of stored terms           */
static idx_t  seq_cap  = 0;     /* current capacity of seq_arr      */

/* Ensure seq_arr can hold at least 'need' elements  */
static void grow_seq_arr(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < need) new_cap <<= 1;
    seq_arr = mem_resize(seq_t, seq_arr, new_cap);
    if (!seq_arr) abort();
    seq_cap = new_cap;
}

/* Current candidate odd multiple of 3 (3, 9, 15, 21, ...) */
static seq_t candidate = 3;

/* Return the n-th term (n ≥ 1).  The sequence is 1‑indexed. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    grow_seq_arr(n);

    while (seq_size < n) {
        /* Test whether 'candidate' satisfies the definition  */
        bool bad = false;
        seq_t pow3 = 1;                /* 3^0 */
        while (pow3 < candidate) {
            seq_t diff = candidate - pow3;

            /* p = candidate - 3^m - 1  →  diff - 1  */
            if (diff >= 3) {
                seq_t p1 = diff - 1;
                if (is_prime_big(p1)) { bad = true; break; }
            }
            /* p = candidate - 3^m + 1  →  diff + 1  */
            if (diff >= 1) {
                seq_t p2 = diff + 1;
                if (is_prime_big(p2)) { bad = true; break; }
            }

            pow3 = pow3 * 3;
        }

        if (!bad) {
            seq_arr[seq_size++] = candidate;
        }

        /* Next odd multiple of 3   (increase by 6) */
        candidate += 6;
    }

    return seq_arr[n - 1];
}
