/*
 * A242811  a(n) is the largest n-digit number whose truncation after its
 *          first k digits is divisible by the k-th Lucas number (A000032(n))
 *          for k = 1..n.
 * https://oeis.org/A242811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) - largest n‑digit number whose first k digits are
 * divisible by the k‑th Lucas number (A000032).  The number is
 * built digit by digit, always trying the largest possible digit that
 * keeps the condition for the next position satisfiable.  If no
 * such number exists, 0 is returned. */

/* The function has external linkage as requested. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static bool build_digits(idx_t pos, idx_t n, seq_t prefix, seq_t *lucas, uint8_t *digits) {
    if (pos > n) return true;

    seq_t Lk = lucas[pos];          /* Lucas number for this position   */
    seq_t rem = (prefix * 10) % Lk; /* 10 * current prefix modulo Lk   */
    seq_t r   = (Lk - rem) % Lk;   /* required residue for digit       */

    if (r > 9) return false;        /* no digit in 0..9 can satisfy     */

    /* Switch to ordinary integers for the very small numbers involved. */
    int r_int   = (int)r;           /* r <= 9 */
    int d_start;

    if (Lk <= 9) {                 /* more than one admissible digit  */
        int step = (int)Lk;
        d_start = r_int + ((9 - r_int) / step) * step;
        for (int d = d_start; d >= r_int; d -= step) {
            digits[pos-1] = (uint8_t)d;
            seq_t next = prefix * 10 + (seq_t)d;
            if (build_digits(pos+1, n, next, lucas, digits))
                return true;
        }
    } else {                        /* unique admissible digit          */
        d_start = r_int;
        digits[pos-1] = (uint8_t)d_start;
        seq_t next = prefix * 10 + (seq_t)d_start;
        if (build_digits(pos+1, n, next, lucas, digits))
            return true;
    }
    return false;                   /* no choice worked at this level    */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1                */

    /* Allocate array of Lucas numbers 1..n */
    seq_t *lucas = (seq_t*)malloc((size_t)(n+1) * sizeof(seq_t));
    if (!lucas) return 0;

    /* Lucas sequence: L0=2, L1=1, hence L1=1, L2=3, */
    lucas[1] = (seq_t)1;
    if (n >= 2) lucas[2] = (seq_t)3;
    for (idx_t i=3; i<=n; ++i) lucas[i] = lucas[i-1] + lucas[i-2];

    /* Buffer for the digits of the final number */
    uint8_t *digits = (uint8_t*)malloc((size_t)n * sizeof(uint8_t));
    seq_t result = 0;

    if (build_digits(1, n, (seq_t)0, lucas, digits)) {
        /* Assemble the final number from its digits */
        for (idx_t i=0; i<n; ++i) {
            result = result * 10 + digits[i];
        }
    } else {
        result = 0;   /* no n‑digit number satisfies the conditions   */
    }

    free(lucas);
    free(digits);
    return result;
}
