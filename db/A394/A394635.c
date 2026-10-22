/*
 * A394635  Isolated totient numbers: even totient numbers k such that k-2 and
 *          k+2 are not totient numbers.
 * https://oeis.org/A394635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Search for isolated totient numbers by sieving totients up to
 * an increasing limit until at least n terms are found.
 *
 * An isolated totient is an even number k that is a value of Euler's
 * totient function φ(m) and for which k−2 and k+2 are NOT totient values.
 * The sequence a(n) is the nth isolated totient.
 */

/* ---- required headers ------------------------------------------------ */
#include <stdlib.h>   /* malloc, free, realloc */
#include <stdint.h>   /* uint32_t */
#include <stdbool.h>  /* bool, true, false */
#include <stddef.h>   /* size_t */

/* ---- static memoisation data ------------------------------------------ */
static seq_t *isolated = NULL;   /* stored isolated totients */
static size_t isolated_len = 0;  /* number of stored terms            */
static idx_t current_limit = 0;  /* last limit used for sieving       */

/* ---- helper: extend the sieve until n isolated values are found -------- */
static void compute_until(idx_t n)
{
    size_t limit = (size_t)current_limit;
    if (limit == 0) limit = 100000;               /* start from modest size */

    while (1) {
        /* Allocate array of flags for totient values */
        bool *is_tot = (bool *)malloc((limit + 3) * sizeof(bool));
        if (!is_tot) abort();

        /* initialise to false */
        for (size_t i = 0; i <= limit + 2; ++i) is_tot[i] = false;

        /* ---- compute φ(i) for 0≤i≤limit -------------------------------- */
        uint32_t *phi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!phi) {
            free(is_tot);
            abort();
        }

        /* initialise φ(i) ← i */
        for (size_t i = 0; i <= limit; ++i) phi[i] = (uint32_t)i;

        /* sieve for primes and update φ */
        for (size_t p = 2; p <= limit; ++p) {
            if (phi[p] == (uint32_t)p) {          /* p is prime            */
                /* for all multiples of p subtract φ/p */
                for (size_t j = p; j <= limit; j += p) {
                    phi[j] -= phi[j] / (uint32_t)p;
                }
            }
        }

        /* mark all totient values up to limit+2 */
        for (size_t i = 1; i <= limit; ++i) {
            uint32_t v = phi[i];
            if (v <= limit + 2) {
                is_tot[v] = true;
            }
        }
        free(phi);

        /* ---- collect isolated even totients --------------------------- */
        size_t cap = 16;
        seq_t *arr = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!arr) {
            free(is_tot);
            abort();
        }
        size_t cnt = 0;

        for (size_t k = 4; k <= limit; k += 2) { /* only even k */
            if (!is_tot[k]) continue;           /* not a totient          */
            bool left  = (k >= 2) ? is_tot[k - 2] : false;
            bool right = (k + 2 <= limit + 2) ? is_tot[k + 2] : false;
            if (!left && !right) {
                if (cnt >= cap) {
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(arr, cap * sizeof(seq_t));
                    if (!tmp) {
                        free(is_tot);
                        free(arr);
                        abort();
                    }
                    arr = tmp;
                }
                arr[cnt++] = (seq_t)k;
            }
        }

        free(is_tot);

        /* Did we collect enough terms? */
        if (cnt >= (size_t)n) {
            /* Replace old cache with the new one */
            free(isolated);
            isolated      = arr;
            isolated_len  = cnt;
            current_limit = (idx_t)limit;
            return;
        }

        /* Not enough terms – enlarge limit and retry */
        free(arr);
        limit <<= 1;  /* double the limit */
    }
}

/* ---- public interface -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (isolated == NULL || isolated_len < (size_t)n) {
        compute_until(n);
    }
    return isolated[(size_t)n - 1];
}
