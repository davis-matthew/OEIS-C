/*
 * A391552  Triangle read by rows: T(n,k) = Sum_{j=0..k} (-1)^(k-j) *
 *          binomial(k,j) * (3+j)^n.
 * https://oeis.org/A391552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 81, u64 n <= 198,
 *            u128 n <= 514, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ballot sequence A391552 :
   a(n) is the (n‑th element) of the triangle read by rows
   where T(n,k) = Sum_{j=0..k} (-1)^(k-j) * binomial(k,j) * (3+j)^n.
   For an index n we find the row r and column k such that
   r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n – r(r+1)/2,
   then a(n) = T(r,k).  All arithmetic uses seq_t, idx_t
   (seq_t may be uint8_t … uint128_t or an arbitrary precision
   class; idx_t is uint64_t).  No external libraries are needed
   beyond the definition of seq_t and idx_t.  */

/* standard headers for types */
#include <stdint.h>
#include <stddef.h>

/* integer square‑free utilities are not required – we compute
   the row by simple linear search because r is small (≤ 140 for n ≤ 10000). */

/* ----------------------------------------------------------------
   Helper: binomial coefficient C(n,k) for idx_t arguments.
   Uses the multiplicative formula; division is exact.
   ---------------------------------------------------------------- */
static seq_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);   /* (n‑k+i) fits in idx_t, cast to seq_t */
        res = res * num / (seq_t)i;        /* division is exact */
    }
    return res;
}

/* ----------------------------------------------------------------
   Helper: integer power of a seq_t base to idx_t exponent.
   Binary exponentiation – uses only +,-,*,/ and shift.
   ---------------------------------------------------------------- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

/* ----------------------------------------------------------------
   Compute T(n,k) from its definition.
   Sum over j = 0..k of (-1)^(k-j) * C(k,j) * (3+j)^n.
   Positive terms are accumulated separately from negative terms
   to avoid intermediate underflow of unsigned seq_t.
   ---------------------------------------------------------------- */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    for (idx_t j = (idx_t)0; j <= k; ++j) {
        seq_t term = binomial_idx(k, j);
        seq_t base = (seq_t)3 + (seq_t)j;
        term = term * pow_seq(base, n);
        if (((k - j) & (idx_t)1) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;   /* pos ≥ neg for all n,k */
}

/* ----------------------------------------------------------------
   Find the row r for a given linear index idx.
   r is the largest integer such that r(r+1)/2 ≤ idx.
   Since r is small for the required range (≤ 140 for idx ≤ 10000),
   a linear search is sufficient and fast.
   ---------------------------------------------------------------- */
static idx_t row_of(idx_t idx)
{
    idx_t r = (idx_t)0;
    /* triangular number for (r+1) is (r+1)(r+2)/2 */
    while (((r + (idx_t)1) * (r + (idx_t)2)) / (idx_t)2 <= idx)
        ++r;
    return r;
}

/* ----------------------------------------------------------------
   Main function requested by the test harness.
   ---------------------------------------------------------------- */
seq_t a(idx_t idx)
{
    idx_t r = row_of(idx);
    idx_t base = r * (r + (idx_t)1) / (idx_t)2;
    idx_t k = idx - base;
    return compute_T(r, k);
}
