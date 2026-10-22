/*
 * A386277  For n >= 0, a(n) is the least Fibonacci number F_i (A000045) such
 *          that for some k >= 0, F_i = (n + 1)*(n + 2*k)/2, or a(n) = -1 if
 *          no such k exists.
 * https://oeis.org/A386277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 21, i32 n <= 43, i64 n <= 73, i128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386277: a(n) is the least Fibonacci number F_i that equals
   (n+1)*(n+2*k)/2 for some k ≥ 0; if no such k exists, a(n) = -1.  */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow during arithmetic. */
    seq_t nSeq = (seq_t)n;
    /* Modulus d = n + 1.  For n = UINT64_MAX this overflows to 0,
       but such a case is outside the intended input range. */
    seq_t d = nSeq + 1;
    if (d == (seq_t)0)        /* overflow guard: no modulus defined */
        return (seq_t)-1;

    /* Lower bound of the arithmetic progression. */
    seq_t L = nSeq * (nSeq + 1) / 2;

    /* Residue that a Fibonacci number must have modulo d. */
    seq_t r = L % d;

    /* Special case n = 0: the sequence starts with 0. */
    if (nSeq == 0)
        return (seq_t)0;

    /* ----- Check whether the residue r actually appears in the Pisano
     * period of Fibonacci numbers modulo d. -------------------------- */
    bool foundResidue = (r == 0);          /* F0 = 0 always present */
    seq_t prev_mod = 0 % d;                /* 0 */
    seq_t curr_mod = 1 % d;                /* 1 */

    /* Detect the Pisano period by observing the return to 0,1. */
    /* We stop after the first completion of the cycle.           */
    for (idx_t i = 0; ; ++i) {
        seq_t next_mod = (prev_mod + curr_mod) % d;
        prev_mod = curr_mod;
        curr_mod = next_mod;

        /* If we looped back to 0,1 (after the initial value) the
           period has ended.  Break only after at least one step.   */
        if (prev_mod == 0 && curr_mod == 1 && i > 0)
            break;

        if (curr_mod == r)
            foundResidue = true;
    }

    if (!foundResidue)
        return (seq_t)-1;

    /* ----- Find the smallest Fibonacci number ≥ L with residue r. ---- */
    seq_t f_prev = 0;
    seq_t f_curr = 1;
    seq_t mod_prev = 0 % d;
    seq_t mod_curr = 1 % d;

    if (f_prev >= L && mod_prev == r)
        return f_prev;
    if (f_curr >= L && mod_curr == r)
        return f_curr;

    for (;;) {
        seq_t f_next   = f_prev + f_curr;
        seq_t mod_next = (mod_prev + mod_curr) % d;

        f_prev = f_curr;
        f_curr = f_next;
        mod_prev = mod_curr;
        mod_curr = mod_next;

        if (f_curr >= L && mod_curr == r)
            return f_curr;
    }

    /* Should never reach here; just in case. */
    return (seq_t)-1;
}
