/*
 * A265747  Numbers written in Jacobsthal greedy base.
 * https://oeis.org/A265747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal integer whose digits are the coefficients of
 *            the greedy Jacobsthal representation of n (digits 0, 1, or 2).
 *            a(0) = 0
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 1364, u64 n <= 8745,
 *            u128 n <= 8745, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the Jacobsthal numbers that are
   needed for a given n and then forms the desired decimal digits. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t jacob[256];          /* Jacobsthal numbers J[0..255]   */
static idx_t jac_len = 0;        /* Number of entries computed      */
static bool jac_init = false;    /* Whether the array has been seeded */

/* Ensure that jacob[0..jac_len-1] are valid and extend the list
   until the next Jacobsthal number would exceed the supplied bound n. */
static void ensure_jacobials(idx_t n)
{
    if (!jac_init) {
        jacob[0] = (seq_t)0;
        jacob[1] = (seq_t)1;
        jac_len = 2;
        jac_init = true;
    }
    /* Keep generating J[i] while the next value would not exceed n. */
    while (jac_len < 256) {
        seq_t next = jacob[jac_len-1] + (seq_t)2 * jacob[jac_len-2];
        if (next > (seq_t)n) break;
        jacob[jac_len] = next;
        jac_len++;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* a(0) = 0 */

    /* Extend the Jacobsthal table as far as needed for this n. */
    ensure_jacobials(n);

    /* Find the largest index i with J[i] <= n. */
    idx_t max_idx = (idx_t)1;
    for (int i = (int)jac_len - 1; i >= 1; --i) {
        if (jacob[i] <= (seq_t)n) {
            max_idx = (idx_t)i;
            break;
        }
    }

    /* Greedy decomposition of n into Jacobsthal numbers. */
    seq_t remaining = (seq_t)n;
    seq_t result = (seq_t)0;
    for (int i = (int)max_idx; i >= 2; --i) {
        seq_t d = remaining / jacob[i];
        if (d > (seq_t)2) d = (seq_t)2;
        remaining -= d * jacob[i];
        result = result * (seq_t)10 + d;
    }
    /* For n = 1 or 2 the loop above handles them correctly. */
    return result;
}
