/*
 * A255608  Numbers n such that 36n+11, 36(n+1)+11, 36(n+2)+11 and 36(n+3)+11
 *          are prime.
 * https://oeis.org/A255608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A255608:
   the n-th integer N such that 36 N+11, 36(N+1)+11, 36(N+2)+11 and
   36(N+3)+11 are all prime.  The function mimics a simple search
   increasing by 5 (since all terms are multiples of 5) and memoises
   previously found terms in a static dynamic array.  It uses the
   provided primality test is_prime_big(seq_t) and the utility
   grow_seq to enlarge the array on demand. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* handle the unfortunate case where the caller asks for 0 */
    if (n == 0) return 0;

    static seq_t *terms = NULL;           /* memoised terms */
    static size_t terms_len = 0;          /* current number of stored terms */
    static size_t terms_cap = 0;          /* allocated capacity */
    static seq_t next_candidate = 5;      /* next multiple of 5 to test */

    /* extend the stored list until we have at least n terms */
    while (terms_len < n) {
        seq_t k = next_candidate;                 /* candidate N */
        seq_t p0 = 36 * k + 11;                    /* 36k+11   */
        seq_t p1 = p0 + 36;                        /* 36(k+1)+11 */
        seq_t p2 = p1 + 36;                        /* 36(k+2)+11 */
        seq_t p3 = p2 + 36;                        /* 36(k+3)+11 */

        if (is_prime_big(p0) && is_prime_big(p1) &&
            is_prime_big(p2) && is_prime_big(p3)) {
            /* store the found term */
            grow_seq(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = k;
        }

        /* move to the next multiple of 5 */
        next_candidate += 5;
    }

    return terms[n - 1];
}
