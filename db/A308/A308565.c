/*
 * A308565  a(n) = Sum_{k=0..n} binomial(n,k) * Stirling1(n,k) * k!.
 * https://oeis.org/A308565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = SUM_{k=0..n} binomial(n,k) * Stirling1(n,k) * k!
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the definition a(n) = n! * [x^n] (1+log(1+x))^n */

#include <stdlib.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------------- */
/* Static state */
/* ------------------------------------------------------------------------- */
static seq_t *fact   = NULL; /* factorial[0..capacity-1]            */
static seq_t *s_prev = NULL; /* Stirling1 of previous row           */
static seq_t *s_curr = NULL; /* temporary Stirling1 row             */
static seq_t *a_arr  = NULL; /* a(n) values                         */
static idx_t  capacity = 0;   /* size of the allocated arrays        */
static idx_t  computed_max = (idx_t)-1; /* last n for which a(n) is known */

/* ------------------------------------------------------------------------- */
/* Grow the internal tables to at least 'need' entries (need >= 0).          */
/* ------------------------------------------------------------------------- */
static void ensure_capacity(idx_t need)
{
    if (need < capacity) return;
    size_t new_cap = (size_t)(need + 1);

    fact   = (seq_t *)realloc(fact,   new_cap * sizeof(seq_t));
    s_prev = (seq_t *)realloc(s_prev, new_cap * sizeof(seq_t));
    s_curr = (seq_t *)realloc(s_curr, new_cap * sizeof(seq_t));
    a_arr  = (seq_t *)realloc(a_arr,  new_cap * sizeof(seq_t));

    /* Initialise new slots to zero */
    for (idx_t i = capacity; i <= need; ++i) {
        fact   [i] = 0;
        s_prev [i] = 0;
        s_curr [i] = 0;
        a_arr  [i] = 0;
    }
    capacity = new_cap;
}

/* ------------------------------------------------------------------------- */
/* Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* First-time initialisation */
    if (computed_max == (idx_t)-1) {
        ensure_capacity(0);
        fact[0]   = (seq_t)1;
        a_arr[0]  = (seq_t)1;
        s_prev[0] = (seq_t)1;          /* S(0,0)=1 */
        computed_max = 0;
    }

    /* Grow arrays if required */
    ensure_capacity(n);

    /* If we already know a(n) just return it */
    if (n <= computed_max) return a_arr[n];

    /* Compute all missing terms up to n */
    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        /* -------- factorial cur! ---------------------------------------- */
        fact[cur] = fact[cur-1] * (seq_t)cur;

        /* -------- Stirling1(cur, k) ------------------------------------ */
        s_curr[0] = 0;                         /* S(cur,0)=0 for cur>0 */
        for (idx_t k = 1; k <= cur; ++k) {
            /* S(n,k) = S(n-1,k-1) - (n-1)*S(n-1,k) */
            seq_t term = s_prev[k];
            s_curr[k] = s_prev[k-1] - (seq_t)(cur-1) * term;
        }

        /* -------- Compute a(cur) --------------------------------------- */
        seq_t sum   = 0;
        seq_t binom = 1;                        /* C(cur,0) */

        for (idx_t k = 0; k <= cur; ++k) {
            /* term = binom(cur,k) * fact[k] * S(cur,k) */
            seq_t term = binom;
            term *= fact[k];
            term *= s_curr[k];
            sum += term;

            /* Update binom for next k: binom = C(cur,k+1) */
            if (k < cur) {
                seq_t num = (cur - k);
                seq_t den = (k + 1);

                seq_t g = gcd(binom, den);
                binom /= g;
                den   /= g;

                g = gcd(num, den);
                num /= g;
                den /= g;

                binom = binom * num;
                if (den != 1)
                    binom /= den;
            }
        }

        a_arr[cur] = sum;

        /* -------- Prepare for next iteration -------------------------- */
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
    }

    computed_max = n;
    return a_arr[n];
}
