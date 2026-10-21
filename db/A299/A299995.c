/*
 * A299995  Number of distinct factorizations of the length-n prefix of the
 *          Fibonacci word A003849 into finite Fibonacci words.
 * https://oeis.org/A299995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct factorizations of the * length‑n
 *            prefix of the Fibonacci word A003849 into * finite Fibonacci
 *            words (prefixes of A003849 of Fibonacci * lengths). The value is
 *            computed by a dynamic programming * routine that grows its
 *            tables only when a larger n is * requested
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/*----- Static tables for memoisation -----------------------------------*/
static idx_t          max_n      = 0;      /* largest n processed so far       */
static seq_t *        dp         = NULL;   /* DP array a[0..max_n]             */
static size_t         fib_cnt    = 0;      /* number of Fibonacci words built  */
static idx_t *        fib_len    = NULL;   /* lengths of Fibonacci words       */
static char **        fib_str    = NULL;   /* word strings for the Fibonacci words */
static char *         word_pref  = NULL;   /* prefix of the infinite Fibonacci word of length max_n */


/*----- Free previously allocated Fibonacci word data ---------------------*/
static void free_fib_words(void)
{
    if (fib_str) {
        for (size_t i = 0; i < fib_cnt; ++i) free(fib_str[i]);
        free(fib_str);
        free(fib_len);
        fib_str = NULL;
        fib_len = NULL;
        fib_cnt = 0;
    }
    if (word_pref) {
        free(word_pref);
        word_pref = NULL;
    }
}


/*----- Build Fibonacci words up to length >= n and the prefix ----------------*/
static void build_fib_words(idx_t n)
{
    free_fib_words();                     /* always start fresh */

    /* Allocate a reasonable maximum for the number of Fibonacci words. */
    size_t alloc = 32;
    fib_len = (idx_t *)malloc(sizeof(idx_t) * alloc);
    fib_str = (char **)malloc(sizeof(char *) * alloc);
    fib_cnt = 0;

    /* f0 = "0" */
    fib_len[fib_cnt] = 1;
    fib_str[fib_cnt] = (char *)malloc(2);
    fib_str[fib_cnt][0] = '0';
    fib_str[fib_cnt][1] = '\0';
    fib_cnt++;

    /* f1 = "01" */
    if (n >= 2) {
        fib_len[fib_cnt] = 2;
        fib_str[fib_cnt] = (char *)malloc(3);
        fib_str[fib_cnt][0] = '0';
        fib_str[fib_cnt][1] = '1';
        fib_str[fib_cnt][2] = '\0';
        fib_cnt++;
    }

    /* Generate further words until the last one has length ≥ n */
    while (fib_len[fib_cnt - 1] < n) {
        if (fib_cnt == alloc) {            /* grow the arrays if necessary */
            alloc <<= 1;
            fib_len = (idx_t *)realloc(fib_len, sizeof(idx_t) * alloc);
            fib_str = (char **)realloc(fib_str, sizeof(char *) * alloc);
        }
        fib_len[fib_cnt] = fib_len[fib_cnt - 1] + fib_len[fib_cnt - 2];
        fib_str[fib_cnt] = (char *)malloc(fib_len[fib_cnt] + 1);
        memcpy(fib_str[fib_cnt], fib_str[fib_cnt - 1], fib_len[fib_cnt - 1]);
        memcpy(fib_str[fib_cnt] + fib_len[fib_cnt - 1],
               fib_str[fib_cnt - 2], fib_len[fib_cnt - 2]);
        fib_str[fib_cnt][fib_len[fib_cnt]] = '\0';
        fib_cnt++;
    }

    /* Build the length‑n prefix of the infinite Fibonacci word */
    word_pref = (char *)malloc(n + 1);
    memcpy(word_pref, fib_str[fib_cnt - 1], n);
    word_pref[n] = '\0';
}


/*----- Main sequence function ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)                     /* defined only for n>=1, but safety */
        return (seq_t)0;

    /* Rebuild tables if the request exceeds the current maximum n. */
    if (n > max_n) {
        if (dp) {                    /* free the old DP array */
            free(dp);
            dp = NULL;
        }
        build_fib_words(n);          /* this frees old word data */

        max_n = n;
        dp = (seq_t *)malloc(sizeof(seq_t) * (max_n + 1));
        dp[0] = (seq_t)1;           /* one way to factor the empty prefix */

        /* initialise the rest to zero */
        for (idx_t i = 1; i <= max_n; ++i)
            dp[i] = (seq_t)0;

        /* Dynamic programming over positions in the prefix. */
        for (idx_t pos = 0; pos < max_n; ++pos) {
            if (dp[pos] == (seq_t)0)
                continue;
            for (size_t k = 0; k < fib_cnt; ++k) {
                idx_t len = fib_len[k];
                if (len > max_n - pos)
                    break;
                if (memcmp(word_pref + pos, fib_str[k], len) == 0)
                    dp[pos + len] += dp[pos];
            }
        }
    }

    return dp[n];
}
