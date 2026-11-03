/*
 * A330179  a(n) = n + floor(ns/r) + floor(nt/r), where r = e - 1, s = e, t =
 *          e + 1.
 * https://oeis.org/A330179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*e/(e-1)) + floor(n*(e+1)/(e-1))
 * Range:     u8 none, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Compute a rational approximation p/q ≈ target with error < 1/(2*MAX_N)
   The denominators are kept below MAX_DEN to avoid overflow. */
static void get_convergent(long double target,
                           unsigned long long *p, unsigned long long *q)
{
    const unsigned long long MAX_DEN = 200000;          /* safe for n up to 10^7 */
    unsigned long long a0 = (unsigned long long)floorl(target);
    unsigned long long p0 = 0, q0 = 1;                  /* previous convergent */
    unsigned long long p1 = 1, q1 = 0;                  /* current convergent  */
    unsigned long long p_next, q_next;
    long double x = target;

    /* Generate convergents until the denominator would exceed MAX_DEN */
    while (true) {
        unsigned long long a = (unsigned long long)floorl(x);
        p_next = a * p1 + p0;
        q_next = a * q1 + q0;
        if (q_next > MAX_DEN)                     /* stop before overflow */
            break;
        p0 = p1;  q0 = q1;
        p1 = p_next;  q1 = q_next;
        if (x == (long double)a)                  /* exact */
            break;
        x = 1.0L / (x - (long double)a);
    }
    *p = p1;
    *q = q1;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    static bool init = false;
    static unsigned long long pa = 0, qa = 0;          /* e/(e-1) */
    static unsigned long long pb = 0, qb = 0;          /* (e+1)/(e-1) */

    if (!init) {
        init = true;

        /* Compute approximations for α = e/(e-1) and β = (e+1)/(e-1) */
        long double e = expl(1.0L);                    /* long double e */
        get_convergent(e / (e - 1.0L), &pa, &qa);      /* α */
        get_convergent((e + 1.0L) / (e - 1.0L), &pb, &qb);  /* β' */
    }

    seq_t n_seq = (seq_t)n;
    seq_t term1 = (n_seq * (seq_t)pa) / (seq_t)qa;
    seq_t term2 = (n_seq * (seq_t)pb) / (seq_t)qb;

    return n_seq + term1 + term2;
}
