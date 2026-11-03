/*
 * A327918  The 16 pure prime dates of each non-leap year of the form
 *          concatenate(month,day) with month and day also prime numbers.
 * https://oeis.org/A327918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = nth pure prime date of a non‑leap year. Computed by
 *            brute‑force search over prime months and prime days and
 *            selecting those concatenations that are prime. The sequence is
 *            periodic with period 16
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: full, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

static const seq_t *init_table(void)
{
    static bool initialized = false;
    static seq_t table[16];

    if (!initialized) {
        const idx_t months[5]       = {2, 3, 5, 7, 11};
        const idx_t prime_days[11]  = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
        const idx_t days_in_month[13] = {0,
                                         31, 28, 31, 30, 31, 30, 31, 31, 30,
                                         31, 30, 31};

        seq_t buffer[16];
        size_t pos = 0;

        for (size_t mi = 0; mi < 5; ++mi) {
            idx_t m      = months[mi];
            idx_t max_day = days_in_month[(int)m];
            for (size_t di = 0; di < 11; ++di) {
                idx_t d = prime_days[di];
                if (d > max_day) break;
                seq_t val = concat_parts(m, d);
                if (is_prime_big(val)) {
                    buffer[pos++] = val;
                }
            }
        }
        for (size_t i = 0; i < pos; ++i) {
            table[i] = buffer[i];
        }
        initialized = true;
    }
    return table;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset but harmless */
    const seq_t *tbl = init_table();
    size_t idx = (size_t)((n - 1) % 16);
    return tbl[idx];
}
