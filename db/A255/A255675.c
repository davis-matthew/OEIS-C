/*
 * A255675  Least integer k such that the n-th prime of form m^2+1 divides the
 *          composite number k^2+1.
 * https://oeis.org/A255675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 for n=1; a(n) = A002496(n)-A005574(n) for n>1 = m_n^2 -
 *            m_n + 1 where m_n is the n-th integer with m_n^2+1 prime
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n) {
    /* static state for memoisation */
    static bool initialized = false;
    static seq_t *answers = NULL;      /* 0‑based array of a-values */
    static size_t capacity = 0;        /* allocated size of answers */
    static size_t len = 0;             /* number of a-values stored */
    static uint64_t last_m = 1;        /* last m that was tested */

    if (!initialized) {
        /* initialise cache with the first term a(1)=3 */
        capacity = 1;
        answers = mem_new(seq_t, capacity);
        if (!answers) return 0; /* memory failure – unlikely in tests */
        answers[0] = 3;
        len = 1;
        initialized = true;
    }

    /* generate terms up to n if needed */
    while (len < n) {
        /* test next m */
        ++last_m;
        seq_t m = (seq_t)last_m;
        seq_t prime_candidate = m * m + 1;
        if (is_prime_big(prime_candidate)) {
            /* compute a-value for this m */
            seq_t val = m * m - m + 1;
            /* grow array if necessary */
            if (len == capacity) {
                capacity = capacity * 2 ? capacity * 2 : 2;
                seq_t *tmp = mem_resize(seq_t, answers, capacity);
                if (!tmp) return 0; /* memory failure */
                answers = tmp;
            }
            answers[len++] = val;
        }
    }
    return answers[n - 1];
}
