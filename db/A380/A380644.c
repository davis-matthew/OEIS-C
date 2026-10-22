/*
 * A380644  Numbers that can be expressed as 4*j*k+j+k, j,k >= 1, as well as
 *          4*j*k-j-k, j,k >= 2.
 * https://oeis.org/A380644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of integers N that can be expressed both as
 * 4*j*k + j + k (j,k ≥ 1) and as 4*j*k – j – k (j,k ≥ 2).
 * The function searches for such N by testing consecutive integers
 * until it has found n terms.  It uses only integer arithmetic. */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square root (floor) for arbitrary‑precision seq_t. */
static seq_t int_sqrt_seq(seq_t x)
{
    seq_t lo = 0, hi = x, mid, r = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        if (mid == 0) { lo = 1; continue; }
        seq_t div = x / mid;
        if (mid <= div) { r = mid; lo = mid + 1; }
        else            { hi = mid - 1; }
    }
    return r;
}

/* Test whether 4*N+1 has
 *   (i) a factor pair of odd numbers ≡ 1 (mod 4) and ≥ 5, and
 *  (ii) a factor pair of odd numbers ≡ 3 (mod 4) and ≥ 7. */
static bool has_both_forms(seq_t P)
{
    seq_t sqrtP = int_sqrt_seq(P);
    bool ok1 = false, ok2 = false;

    for (seq_t d = 1; d <= sqrtP; d += 2) {          /* odd divisors only */
        if (P % d) continue;
        seq_t e = P / d;

        if (!ok1 &&
            (d % 4) == 1 && d >= 5 &&
            (e % 4) == 1 && e >= 5)
            ok1 = true;

        if (!ok2 &&
            (d % 4) == 3 && d >= 7 &&
            (e % 4) == 3 && e >= 7)
            ok2 = true;

        if (ok1 && ok2) return true;
    }
    return false;
}

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cached terms */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of terms stored */
    static idx_t  cur   = 1;      /* next candidate integer to test */

    /* expand cache if needed */
    while (len < n) {
        if (cap < len + 1) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < len + 1) new_cap = len + 1;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* allocation failure */
            terms = tmp;
            cap   = new_cap;
        }

        seq_t P = (seq_t)4 * (seq_t)cur + 1;   /* P = 4*N + 1 */
        if (has_both_forms(P))
            terms[len++] = (seq_t)cur;

        ++cur;
    }

    return terms[n - 1];
}
