/*
 * A272072  Nonnegative integers n such that 10^n is not of the form x^3 + y^3
 *          + z^3 where x > y > z > 0.
 * https://oeis.org/A272072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   sequence of nonnegative integers n such that 10^n cannot be
 *            written as the sum of three distinct positive cubes x^3 + y^3 +
 *            z^3 (x > y > z > 0). An element a(k) is the (k‑1)th such
 *            integer, with offset 1 (a(1) = 0). Because all n > 10 are
 *            representable, the sequence is finite and consists of the...
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute 10^exp for small exp (here exp ≤ 10). */
static uint64_t pow10_u64(idx_t exp)
{
    uint64_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= 10;
    return r;
}

/* Integer cube root (floor) for 64‑bit values. */
static uint64_t int_cuberoot(uint64_t n)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* Test whether a given target value can be expressed as a sum of three
   distinct positive cubes. */
static bool is_representable(uint64_t target)
{
    /* 10^0 = 1 cannot be represented with distinct positive cubes. */
    if (target == 1) return false;

    uint64_t root = int_cuberoot(target);
    /* Allocate array of cubes 1^3 … root^3. */
    uint64_t *cubes = (uint64_t *)malloc((root + 1) * sizeof(uint64_t));
    if (!cubes) return false;           /* unlikely for small n */

    for (uint64_t i = 1; i <= root; ++i) cubes[i] = i * i * i;

    for (uint64_t z = 1; z <= root - 2; ++z) {
        uint64_t zc = cubes[z];
        for (uint64_t y = z + 1; y <= root - 1; ++y) {
            uint64_t sum_z_y = zc + cubes[y];
            if (sum_z_y >= target) continue;
            uint64_t rem = target - sum_z_y;
            /* Binary search for a cube equal to rem with index > y. */
            uint64_t lo = y + 1, hi = root, mid;
            while (lo <= hi) {
                mid = (lo + hi) >> 1;
                if (cubes[mid] == rem) {
                    free(cubes);
                    return true;
                }
                if (cubes[mid] < rem) lo = mid + 1;
                else hi = mid - 1;
            }
        }
    }

    free(cubes);
    return false;
}

/* Build the finite list of non‑representable exponents once. */
#define SEQ_CAP 8
static seq_t seq_terms[SEQ_CAP];
static idx_t seq_len = 0;
static bool seq_built = false;

static void build_sequence(void)
{
    if (seq_built) return;

    for (idx_t n = 0; n <= 10 && seq_len < SEQ_CAP; ++n) {
        uint64_t target = pow10_u64(n);
        if (!is_representable(target)) seq_terms[seq_len++] = (seq_t)n;
    }
    seq_built = true;
}

/* a(idx_t n): return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    build_sequence();
    if (n == 0 || n > seq_len) return 0;
    return seq_terms[n - 1];
}
