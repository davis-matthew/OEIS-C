/*
 * A336752  The 'Look and Say' sequence of the concatenation of the prime
 *          numbers A033308.
 * https://oeis.org/A336752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the Look‑and‑Say sequence of the concatenation of primes
 * (OEIS A336752).  The function walks the infinite digit string
 * formed by concatenating all decimal primes, keeps track of runs of
 * equal digits, and emits the Run length followed by the digit,
 * increasing term_idx as it goes.  The value n is 1‑based. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1; safe fallback */

    idx_t term_idx = 1;             /* next term to output */
    bool have_run = false;          /* whether a run has started */
    unsigned char last_digit = 0;   /* digit at the end of the run */
    seq_t run_len = 0;              /* length of the current run */

    idx_t cand = 2;                 /* first prime to test */

    while (1) {
        /* find the next prime */
        while (!is_prime(cand))
            cand += (cand == 2 ? 1 : 2);

        /* process the decimal digits of the prime (most‑significant first) */
        uint64_t temp = cand;
        unsigned char digits[20];  /* enough for uint64_t */
        int dcnt = 0;
        if (temp == 0) {
            digits[dcnt++] = 0;
        } else {
            while (temp > 0) {
                digits[dcnt++] = (unsigned char)(temp % 10);
                temp /= 10;
            }
        }

        for (int i = dcnt - 1; i >= 0; --i) {
            unsigned char digit = digits[i];

            if (!have_run) {
                have_run = true;
                last_digit = digit;
                run_len   = 1;
                continue;
            }

            if (digit == last_digit) {
                run_len++;                     /* extend current run   */
            } else {
                /* finish the previous run */
                if (term_idx == n) return run_len;
                term_idx++;

                if (term_idx == n) return (seq_t)last_digit;
                term_idx++;

                /* start new run with the current digit */
                last_digit = digit;
                run_len    = 1;
            }
        }

        /* prepare candidate for the next prime */
        cand += (cand == 2 ? 1 : 2);
    }
}
