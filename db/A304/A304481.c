/*
 * A304481  Turn the power-tower for n upside-down.
 * https://oeis.org/A304481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 288, u32 n <= 624, u64 n <= 1295,
 *            u128 n <= 2400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A304481 – Turn the power‑tower for n upside‑down.
    For each positive integer n we uniquely write

        n = c(x1)^(c(x2)^(…^c(xk)))

    where each c(xi) is a number >1 that is not a perfect power (A007916).
    The value a(n) is obtained by evaluating the same tower with the
    c-values reversed, i.e.  a(n) = c(xk)^(c(xk‑1)^(…^c(x1))).

    This implementation computes the decomposition recursively, then
    evaluates the reversed tower by repeated squaring.  All arithmetic is
    performed only with the integer type seq_t.  No external utilities
    are required. */

#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Integer exponentiation – base^exp, exp fits in idx_t (here small). */
/* ------------------------------------------------------------------ */
static seq_t pow_seq(const seq_t base, idx_t exp) {
    seq_t res = 1;
    seq_t b   = base;
    while (exp) {
        if (exp & 1) res *= b;
        b *= b;
        exp >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Bit length of a seq_t value – for small values this is fast.        */
/* ------------------------------------------------------------------ */
static int bitlen_seq_t(seq_t x) {
    int len = 0;
    while (x > (seq_t)1) {
        x >>= 1;
        ++len;
    }
    return len + 1;          /* at least 1 for x>=1 */
}

/* ------------------------------------------------------------------ */
/* Integer e‑th root of n (floor).                                 */
/* ------------------------------------------------------------------ */
static seq_t integer_root(seq_t n, idx_t e) {
    seq_t lo = 1, hi = n;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2;   /* ceil midpoint */
        seq_t p   = pow_seq(mid, e);
        if (p <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* Test whether a number is a perfect power (>=2).                    */
/* ------------------------------------------------------------------ */
static bool is_perfect_power(seq_t n) {
    if (n <= (seq_t)1) return false;
    int max_e = bitlen_seq_t(n);
    for (int e = max_e; e >= 2; --e) {
        seq_t r = integer_root(n, e);
        if (r > (seq_t)1 && pow_seq(r, e) == n)
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Decompose n into the list of non‑perfect‑power bases c(i).          */
/* Components are stored in out[0..len-1] in the original order.     */
/* ------------------------------------------------------------------ */
static void decompose(seq_t n, seq_t *out, size_t *len) {
    seq_t current = n;
    while (1) {
        bool found = false;
        seq_t base = 0, exp = 0;

        int max_e = bitlen_seq_t(current);
        for (int e = max_e; e >= 2; --e) {
            seq_t r = integer_root(current, e);
            if (r > (seq_t)1 && pow_seq(r, e) == current && !is_perfect_power(r)) {
                base = r;
                exp  = e;
                found = true;
                break;                     /* unique representation   */
            }
        }

        if (!found) {                      /* current itself is not a perfect power */
            out[(*len)++] = current;
            break;
        }

        out[(*len)++] = base;              /* store the base of this level   */
        current = exp;                     /* exponent becomes the new current */
    }
}

/* ------------------------------------------------------------------ */
/* Evaluate the reversed power tower from the component list.          */
/* The components are in original order: c1, c2, …, ck.                 */
/* ------------------------------------------------------------------ */
static seq_t reverse_tower(const seq_t *comp, size_t len) {
    seq_t val = comp[0];
    for (size_t i = 1; i < len; ++i)
        val = pow_seq(comp[i], (idx_t)val);   /* exponent fits in idx_t (≤ n)    */
    return val;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == (idx_t)0)           /* not defined, just return 0 for safety */
        return (seq_t)0;
    if (n == (idx_t)1)           /* a(1) = 1 by definition                      */
        return (seq_t)1;

    /* The power tower depth for n ≤ 10 000 is ≤ 10.                    */
    seq_t comp[64];
    size_t len = 0;

    decompose((seq_t)n, comp, &len);
    return reverse_tower(comp, len);
}
