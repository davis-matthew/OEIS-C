/*
 * A392113  Numbers k such that 6*k - (greatest prime < 6*n) < (least prime >
 *          6*n) - 6*k.
 * https://oeis.org/A392113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = nth integer k (k ≥ 1) such that
 *   6*k - p1  <  p2 - 6*k
 * where p1 = greatest prime < 6*k,  p2 = least prime > 6*k.
 * The sequence starts with 4, 8, 9, 14, … (offset 1).
 */

#include <stdint.h>
#include <stdbool.h>

/* ---------- prime utilities ---------------------------------- */

/* deterministic primality test for 64‑bit unsigned integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* greatest prime strictly less than x (x >= 6) */
static uint64_t prev_prime(uint64_t x)
{
    if (x <= 3) return 2;              /* only 2 is < 3, but x≥6 in usage */
    uint64_t v = x - 1;
    while (v >= 2) {
        if (is_prime_uint64(v)) return v;
        --v;
    }
    return 2;
}

/* least prime strictly greater than x (x >= 5) */
static uint64_t next_prime(uint64_t x)
{
    uint64_t v = x + 1;
    while (!is_prime_uint64(v)) ++v;
    return v;
}

/* ---------- main sequence function --------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; offset is 1 */

    idx_t found = 0;     /* how many terms found so far */
    uint64_t k = 1;      /* candidate k to test */

    while (found < n) {
        uint64_t sixk = 6 * k;
        uint64_t p_low  = prev_prime(sixk);
        uint64_t p_high = next_prime(sixk);

        /* Compare distances */
        if (sixk - p_low < p_high - sixk) {
            ++found;
            if (found == n) return (seq_t)k;
        }
        ++k;
    }

    /* Should never reach here */
    return (seq_t)0;
}
