/*
 * A331802  Integers having no representation as sum of two nonsquarefree
 *          numbers.
 * https://oeis.org/A331802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   a(n) is the nth integer that cannot be written as a sum of two
 *            nonsquarefree numbers. The sequence is finite; any n larger than
 *            the number of terms returns 0
 * Range:     u8 none, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* The build supplies the types seq_t and idx_t.  Do not redefine them. */

/* Limit for the pre‑computation.  200 is more than enough to locate all
   14 terms of A331802 (the largest term is 23). */
#define LIMIT 200
#define MAX_TERMS 20   /* space for the computed terms */

static seq_t  terms[MAX_TERMS];
static idx_t  term_count = 0;
static bool   initialized = false;

seq_t a(idx_t n)
{
    /* 0‑based guard – the sequence is defined only for n >= 1 */
    if (n == 0) return 0;

    if (!initialized) {
        /* Mark numbers that are not squarefree (nonsquarefree).  */
        bool is_nsf[LIMIT + 1] = {0};
        for (int i = 2; i <= LIMIT; ++i) {
            bool nonsq = false;
            for (int j = 2; j * j <= i; ++j) {
                if (i % (j * j) == 0) { nonsq = true; break; }
            }
            is_nsf[i] = nonsq;
        }

        /* Collect all nonsquarefree numbers up to LIMIT.  */
        seq_t nsf[LIMIT + 1];
        int nsf_len = 0;
        for (int i = 2; i <= LIMIT; ++i)
            if (is_nsf[i]) nsf[nsf_len++] = (seq_t)i;

        /* Find all representable sums of two nonsquarefree numbers.  */
        bool rep[LIMIT + 1] = {0};
        for (int i = 0; i < nsf_len; ++i) {
            for (int j = i; j < nsf_len; ++j) {
                seq_t sum = nsf[i] + nsf[j];
                if (sum <= LIMIT) rep[(int)sum] = true;
            }
        }

        /* Record numbers that are NOT representable.  */
        for (int x = 1; x <= LIMIT; ++x) {
            if (!rep[x]) {
                if (term_count < MAX_TERMS)
                    terms[term_count++] = (seq_t)x;
                else
                    break;   /* safety – should not happen for LIMIT=200 */
            }
        }

        initialized = true;
    }

    /* Return the requested term if it exists, otherwise 0. */
    if (n <= term_count) return terms[n - 1];
    return 0;
}
