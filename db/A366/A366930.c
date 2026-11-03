/*
 * A366930  a(n) is the smallest odd composite k such that n^((k+1)/2) == n
 *          (mod k).
 * https://oeis.org/A366930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd composite k such that n^((k+1)/2) ≡ n (mod
 *            k)
 * Range:     u8 n <= 1, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include <string.h>
#include "../utils/powmod.h"
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

seq_t a(idx_t n)
{
    /* Pre‑computed odd composite list up to 1729 (largest needed k) */
    static seq_t *odd_comps = NULL;
    static size_t odd_comp_cnt = 0;

    if (!odd_comps) {
        const idx_t NMAX = 1729;
        const size_t ARRAY_LEN = (NMAX + 1) / 2;   /* enough space for all odd numbers */

        odd_comps = mem_new(seq_t, ARRAY_LEN);
        if (!odd_comps)
            return 0;  /* allocation failed – should not happen in tests */

        for (idx_t k = 9; k <= NMAX; k += 2) {
            if (!is_prime_big(k)) {          /* odd composite */
                odd_comps[odd_comp_cnt++] = (seq_t)k;
            }
        }
    }

    seq_t val = (seq_t)n;

    for (size_t i = 0; i < odd_comp_cnt; ++i) {
        seq_t k = odd_comps[i];
        uint64_t exp = (uint64_t)((k + 1) / 2);
        seq_t r = powmod(val, exp, k);
        if (r == (val % k))
            return k;
    }

    /* Theoretical guarantee: a(n) ≤ 1729, so we should never reach here */
    return 0;
}
