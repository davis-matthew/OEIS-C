/*
 * A242397  a(n) is the number of different bases b such that the Brazilian
 *          numbers A125134(n) remain a repdigit number.
 * https://oeis.org/A242397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the number of bases b such that
   the n-th Brazilian number (A125134(n)) is a repdigit in base b.
   The implementation enumerates Brazilian numbers in increasing order,
   counting admissible bases for each. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for idx_t values. */
static inline idx_t integer_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Count bases for which m is a repdigit.  Returns the count as seq_t. */
static inline seq_t count_bases(idx_t m)
{
    if (m < 3) return 0;           /* no admissible bases */
    idx_t sqrt_m = integer_sqrt(m);
    idx_t cnt = 0;

    /* Bases up to sqrt_m: test all digits. */
    for (idx_t b = 2; b <= sqrt_m; ++b) {
        idx_t temp = m;
        idx_t first = temp % b;
        bool ok = true;
        while (temp) {
            if (temp % b != first) { ok = false; break; }
            temp /= b;
        }
        if (ok) ++cnt;
    }

    /* Bases larger than sqrt_m produce at most two digits:
       m = d * (b + 1)  with 1 < d < b.  Enumerate divisors d of m. */
    for (idx_t d = 1; d <= sqrt_m; ++d) {
        if (m % d != 0) continue;
        /* Divisor d as repeated digit. */
        if (d > 1) {
            idx_t b = m / d - 1;
            if (b > sqrt_m && b >= 2 && b < m - 1 && d < b) ++cnt;
        }
        /* Reciprocal divisor d2 = m / d. */
        idx_t d2 = m / d;
        if (d2 != d && d2 > 1) {
            idx_t b = m / d2 - 1;
            if (b > sqrt_m && b >= 2 && b < m - 1 && d2 < b) ++cnt;
        }
    }
    return (seq_t)cnt;
}

/* Ensure the array capacity. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Static storage for computed sequence values. */
static seq_t *counts   = NULL;   /* array of a(n) values      */
static size_t count_len = 0;     /* number of values stored   */
static size_t count_cap = 0;     /* allocated capacity        */
static idx_t known_max_m = 0;    /* last integer m processed  */

/* Generate Brazilian numbers until we have at least "target" entries. */
static void generate_until(size_t target)
{
    while (count_len < target) {
        ++known_max_m;                        /* next m to test         */
        seq_t c = count_bases(known_max_m);   /* bases for this m       */
        if (c) {                              /* m is Brazilian         */
            ensure_capacity(count_len + 1, &count_cap, &counts);
            counts[count_len++] = c;
        }
    }
}

/* Public entry point: a(n) for the OEIS sequence A242397. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for invalid index */
    generate_until((size_t)n);
    return counts[n - 1];
}
