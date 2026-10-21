/*
 * A272341  Numbers n such that 6^n is not of the form (x^3 + y^3 + z^3) / 3
 *          where x > y > z > 0.
 * https://oeis.org/A272341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A272341: a(n) is the nth number n (offset 1) such that
   6^n is NOT the average of three distinct positive cubes.
   The sequence is finite – all terms are ≤ 15 – and this implementation
   finds them by brute‑force search on the fly and memoises the list. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *terms      = NULL;   /* list of sequence values */
static size_t term_count = 0;      /* number of values stored */
static uint64_t *cubes   = NULL;   /* array of i^3 for i = 0 .. max_cube */
static idx_t max_cube = 0;         /* largest i with i^3 ≤ 3·6^15 */

/* integer power: base^exp  (exp ≤ 15, result fits in uint64_t) */
static uint64_t pow_uint64(uint64_t base, idx_t exp)
{
    uint64_t r = 1;
    while (exp--) r *= base;
    return r;
}

/* pre‑compute all i^3 ≤ 3·6^15 */
static void init_cubes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    uint64_t max_target = 3 * pow_uint64(6, 15);   /* ≈1.4·10¹² */
    max_cube = 0;
    /* count cubes up to max_target */
    while (true) {
        uint64_t v = (uint64_t)(max_cube + 1) * (max_cube + 1) * (max_cube + 1);
        if (v > max_target) break;
        ++max_cube;
    }

    cubes = (uint64_t *)malloc((max_cube + 1) * sizeof(uint64_t));
    cubes[0] = 0;
    for (uint64_t i = 1; i <= max_cube; ++i)
        cubes[i] = i * i * i;
}

/* true iff 6^n can be written as the average of three distinct positive cubes */
static bool can_represent(idx_t n)
{
    uint64_t target = 3 * pow_uint64(6, n);   /* target = 3·6^n, fits in uint64_t for n ≤ 15 */
    for (uint64_t z = 1; z <= max_cube && cubes[z] < target; ++z) {
        for (uint64_t y = z + 1; y <= max_cube &&
                cubes[z] + cubes[y] < target; ++y) {
            uint64_t rem = target - cubes[z] - cubes[y];
            uint64_t x = (uint64_t)pow_uint64(6, 1); /* dummy to keep compiler happy */
            /* find cube root of rem */
            uint64_t xr = 0;
            /* integer cube root by binary search */
            uint64_t lo = 1, hi = max_cube;
            while (lo <= hi) {
                uint64_t mid = (lo + hi) >> 1;
                uint64_t m3 = mid * mid * mid;
                if (m3 == rem) { xr = mid; break; }
                if (m3 < rem) lo = mid + 1;
                else hi = mid - 1;
            }
            if (xr == 0) continue;          /* rem not a perfect cube */
            if (xr > y) return true;        /* distinct and ordered */
        }
    }
    return false;
}

/* compute and store all terms (0 ≤ n ≤ 15) */
static void init_sequence(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    init_cubes();

    terms = (seq_t *)malloc(16 * sizeof(seq_t));
    for (idx_t n = 0; n <= 15; ++n) {
        if (!can_represent(n))
            terms[term_count++] = (seq_t)n;
    }
}

/* public function: a(n) returns the nth term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined index 0, return 0 */
    init_sequence();
    if ((idx_t)n - 1 < term_count)
        return terms[(idx_t)n - 1];
    /* sequence has only 10 terms; return 0 for larger indices */
    return 0;
}
