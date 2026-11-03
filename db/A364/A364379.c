/*
 * A364379  Greedy Jacobsthal-Niven numbers: numbers that are divisible by the
 *          sum of the digits in their representation in Jacobsthal greedy
 *          base (A265747).
 * https://oeis.org/A364379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9914 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Greedy Jacobsthal‑Niven numbers by scanning integers
   and checking that the number is divisible by the sum of its
   digits in greedy Jacobsthal base (A265747). */

#include <stddef.h>  /* size_t */

static seq_t
jacob_digit_sum(seq_t k)
{
    /* Jacobsthal numbers up to k (at most ~logφ(k) ≈ 64 terms for
       64‑bit integers, less for smaller ones). No dynamic allocation. */
    seq_t w[64];
    size_t wlen = 2;          /* w[0]=0, w[1]=1 */
    w[0] = (seq_t)0;
    w[1] = (seq_t)1;
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;

    while (1) {
        seq_t next = a1 + (seq_t)2 * a0;
        /* stop if next overflows or exceeds k */
        if (next <= k) {
            if (wlen < sizeof(w) / sizeof(w[0])) {
                w[wlen++] = next;
                a0 = a1;
                a1 = next;
            } else {
                /* w is full, but next is still ≤ k – we cannot proceed.
                   In practice k is small enough that this never happens. */
                break;
            }
        } else {
            break;
        }
    }

    seq_t sum = 0;
    for (size_t i = wlen; i-- > 1; ) {
        seq_t weight = w[i];
        seq_t coeff = k / weight;
        sum += coeff;
        k %= weight;
        if (k == 0) break;
    }
    return sum;
}

seq_t
a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 1;
    while (1) {
        seq_t s = jacob_digit_sum(k);
        if (s != 0 && (k % s) == 0) {
            ++found;
            if (found == n) return k;
        }
        ++k;
    }
}
