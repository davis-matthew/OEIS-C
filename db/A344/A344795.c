/*
 * A344795  Numbers that are the sum of five squares in two or more ways.
 * https://oeis.org/A344795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 219, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A344795: numbers N that are the sum of five positive
 * squares in two or more distinct (unordered) ways.
 *
 * The code generates the sequence by a simple sieve:
 *    For each integer N starting from 1, count the unordered
 *    representations of N as a^2+b^2+c^2+d^2+e^2 with
 *    1 ≤ a ≤ b ≤ c ≤ d ≤ e.  If at least two such representations
 *    exist, N is appended to the list of terms.
 *
 *  a(1) = 20, a(2) = 29, a(3) = 32, …
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for idx_t (uint64_t) */
static inline idx_t floor_sqrt(idx_t n) {
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Count unordered representations of N as sum of 5 positive squares.
 * Returns the count but exits early once it reaches 2 (no need for
 * larger counts).  Representation condition: 1 ≤ a ≤ b ≤ c ≤ d ≤ e,
 * a^2 + b^2 + c^2 + d^2 + e^2 == N.
 */
static unsigned short count_reps(idx_t N) {
    unsigned short cnt = 0;
    idx_t a_max = floor_sqrt(N / 5ull);          /* a^2 ≤ N/5 ensures room for 5 equal squares */
    if (a_max == 0) return 0;                    /* N < 5 gives no positive-square representation */
    for (idx_t a = 1; a <= a_max; ++a) {
        idx_t rem1 = N - a * a;
        idx_t b_max = floor_sqrt(rem1 / 4ull);
        for (idx_t b = a; b <= b_max; ++b) {
            idx_t rem2 = rem1 - b * b;
            idx_t c_max = floor_sqrt(rem2 / 3ull);
            for (idx_t c = b; c <= c_max; ++c) {
                idx_t rem3 = rem2 - c * c;
                idx_t d_max = floor_sqrt(rem3 / 2ull);
                for (idx_t d = c; d <= d_max; ++d) {
                    idx_t rem4 = rem3 - d * d;
                    idx_t e = floor_sqrt(rem4);
                    if (e < d) continue;      /* maintain ordering e ≥ d */
                    if (rem4 == e * e) {
                        ++cnt;
                        if (cnt >= 2) return cnt;   /* early exit */
                    }
                }
            }
        }
    }
    return cnt;
}

/* Dynamic array holding the terms of the sequence.  Stored in 0‑based
 * order: terms[0] is a(1). */
static seq_t *terms   = NULL;
static size_t terms_l = 0;   /* number of terms stored */
static size_t terms_c = 0;   /* capacity of the array */
static idx_t  last_checked = 0;  /* last integer examined by the sieve */

/* Append a new term to the dynamic array, expanding capacity when needed. */
static void append_term(seq_t val) {
    if (terms_l == terms_c) {
        size_t new_c = terms_c ? terms_c * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_c * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_c = new_c;
    }
    terms[terms_l++] = val;
}

/* Ensure that at least n terms of the sequence have been computed. */
static void ensure_terms(idx_t n) {
    while (terms_l < n) {
        ++last_checked;
        if (count_reps(last_checked) >= 2) {
            append_term((seq_t)last_checked);
        }
    }
}

/* Public function required by the build.  External linkage.  Returns
 * a(n) for any n ≥ 1.  The offset of the sequence is 1. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* harmless fallback for n=0 */
    ensure_terms(n);
    return terms[n - 1];
}
