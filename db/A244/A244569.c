/*
 * A244569  Number of values s such that n is in the trajectory of the
 *          'Reverse and Add!' sequence starting with s.
 * https://oeis.org/A244569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 3211, u16 n <= 9712, u32 n <= 9712, u64 n <= 9712,
 *            u128 n <= 9712, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of starting values s such that n appears in the
 * trajectory of the Reverse-and-Add sequence.
 *   a(n) = |{ s ≥ 0 : ∃k ≥ 0  with  f⁽ᵏ⁾(s) = n }|,
 *   where f(s) = s + reverse(s).
 * This implementation builds the pre‑image tree of n recursively.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t count_seeds(seq_t n);
static void backtrack_preimage(int pos, seq_t carry, int k,
                               const int *ndigs, int ndig_len,
                               int *s, seq_t *sum_total);

/* Public function required by the sequence framework */
seq_t a(idx_t n)
{
    return count_seeds((seq_t)n);
}

/* Recursive counter: returns the number of seeds that eventually reach n */
static seq_t count_seeds(seq_t n)
{
    /* Base case: n == 0 reaches only itself */
    if (n == (seq_t)0) {
        return (seq_t)1;
    }

    seq_t total = (seq_t)1;   /* count the seed s = n itself */

    /* Extract decimal digits of n (least significant first) */
    int ndigs[128];
    int ndig_len = 0;

    if (n == (seq_t)0) {
        ndigs[ndig_len++] = 0;
    } else {
        seq_t tmp = n;
        while (tmp != (seq_t)0) {
            ndigs[ndig_len++] = (int)(tmp % (seq_t)10);
            tmp = tmp / (seq_t)10;
        }
    }

    /* For each possible length of predecessor s (1 .. ndig_len) */
    for (int k = 1; k <= ndig_len; ++k) {
        int s_digits[128];
        for (int i = 0; i < k; ++i) s_digits[i] = -1;  /* unknown */

        seq_t partial = (seq_t)0;
        backtrack_preimage(0, (seq_t)0, k, ndigs, ndig_len,
                           s_digits, &partial);
        total += partial;
    }

    return total;
}

/* Recursive backtracking that enumerates all possible predecessors
 * of length k. For each valid predecessor s found, it adds
 * count_seeds(s) to *sum_total.
 */
static void backtrack_preimage(int pos, seq_t carry, int k,
                               const int *ndigs, int ndig_len,
                               int *s, seq_t *sum_total)
{
    if (pos == ndig_len) {
        if (carry != (seq_t)0) return;

        /* All digits of s must be known by now */
        for (int idx = 0; idx < k; ++idx)
            if (s[idx] == -1) return;

        /* Leading digit of s cannot be 0 when k > 1 (unless s == 0) */
        if (k > 1 && s[k - 1] == 0) return;

        /* Build the numeric value of the predecessor */
        seq_t val = (seq_t)0;
        for (int idx = k - 1; idx >= 0; --idx) {
            val = val * (seq_t)10 + (seq_t)s[idx];
        }

        /* Add the count for this predecessor */
        *sum_total += count_seeds(val);
        return;
    }

    int i = pos;
    int j = k - 1 - i;
    int digit_n = ndigs[pos];

    /* Prepare candidate digits for positions i and j */
    int a_vals[10], a_cnt = 0;
    int b_vals[10], b_cnt = 0;
    int a_known = 0, b_known = 0;

    /* Position i */
    if (i >= k) {
        a_known = 1;
        a_vals[0] = 0;
        a_cnt = 1;
    } else if (s[i] != -1) {
        a_known = 1;
        a_vals[0] = s[i];
        a_cnt = 1;
    } else {
        for (int d = 0; d <= 9; ++d) {
            /* Skip leading zero for the most significant digit */
            if (i == k - 1 && k > 1 && d == 0) continue;
            a_vals[a_cnt++] = d;
        }
    }

    /* Position j */
    if (j < 0 || j >= k) {
        b_known = 1;
        b_vals[0] = 0;
        b_cnt = 1;
    } else if (s[j] != -1) {
        b_known = 1;
        b_vals[0] = s[j];
        b_cnt = 1;
    } else {
        for (int d = 0; d <= 9; ++d) {
            /* Skip leading zero for the most significant digit */
            if (j == k - 1 && k > 1 && d == 0) continue;
            b_vals[b_cnt++] = d;
        }
    }

    /* Iterate over all combinations of a and b */
    for (int ai = 0; ai < a_cnt; ++ai) {
        int a = a_vals[ai];
        for (int bi = 0; bi < b_cnt; ++bi) {
            int b = b_vals[bi];

            /* If i == j, digits must be equal (middle digit in odd length) */
            if (i == j && a != b) continue;

            seq_t sum = (seq_t)a + (seq_t)b + carry;
            seq_t sum_mod = sum % (seq_t)10;
            seq_t carry_next = sum / (seq_t)10;

            if (sum_mod != (seq_t)digit_n) continue;

            /* Record assignments if they were not already known */
            int orig_a = -1, orig_b = -1;
            int changed_a = 0, changed_b = 0;

            if (!a_known && i < k) {
                orig_a = s[i];
                s[i] = a;
                changed_a = 1;
            }
            if (!b_known && j >= 0 && j < k) {
                orig_b = s[j];
                s[j] = b;
                changed_b = 1;
            }

            backtrack_preimage(pos + 1, carry_next, k,
                               ndigs, ndig_len, s, sum_total);

            if (changed_a) s[i] = orig_a;
            if (changed_b) s[j] = orig_b;
        }
    }
}
