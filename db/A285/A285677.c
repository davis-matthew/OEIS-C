/*
 * A285677  {0010->2}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A285677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A285677: left‑to‑right replacement of each
   "0010" in the infinite Fibonacci word with a single 2.  The
   function scans the original Fibonacci word, producing the
   transformed sequence on the fly. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char *orig = NULL;        /* buffer holding the original word */
static size_t orig_len = 0;      /* length of orig */

/* Generate the Fibonacci word until it reaches at least 'needed' characters. */
static void ensure_orig_length(size_t needed)
{
    if (orig_len >= needed)
        return;

    /* Build from scratch: start with S0="0", S1="01" */
    char *s0 = (char*)malloc(1);
    char *s1 = (char*)malloc(2);
    if (!s0 || !s1) return;            /* allocation failed */

    s0[0] = '0';
    s1[0] = '0';
    s1[1] = '1';
    size_t len0 = 1;
    size_t len1 = 2;

    /* Build S_k = S_{k-1} + S_{k-2} until len1 >= needed */
    while (len1 < needed) {
        size_t new_len = len1 + len0;
        char *s2 = (char*)malloc(new_len);
        if (!s2) break;                 /* allocation failed */

        memcpy(s2, s1, len1);
        memcpy(s2 + len1, s0, len0);

        free(s0);
        s0 = s1;
        len0 = len1;
        s1 = s2;
        len1 = new_len;
    }

    if (orig) free(orig);
    orig = s1;
    orig_len = len1;
    free(s0);
    return;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset is 1, but guard anyway */

    /* Need enough original characters: worst case 4 original chars per
       transformed char, plus safety margin. */
    size_t needed;
    if (n > SIZE_MAX / 5)            /* avoid overflow */
        needed = SIZE_MAX;
    else
        needed = (size_t)n * 5;

    ensure_orig_length(needed);

    size_t i = 0;                    /* index in original word */
    idx_t  pos = 1;                  /* current output position (1‑based) */

    while (pos <= n) {
        if (i + 3 < orig_len &&
            orig[i]   == '0' &&
            orig[i+1] == '0' &&
            orig[i+2] == '1' &&
            orig[i+3] == '0') {

            if (pos == n)
                return (seq_t)2;   /* replacement produces a 2 */

            i += 4;
            pos++;
        } else {
            if (pos == n)
                return (seq_t)(orig[i] - '0'); /* output original 0 or 1 */

            i++;
            pos++;
        }
    }
    return (seq_t)0;   /* should never reach here */
}
