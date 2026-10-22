/*
 * A329631  Irregular triangle read by rows where row n lists the prime
 *          indices of the n-th squarefree number.
 * https://oeis.org/A329631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence A329631: a(n) = the nth term of the irregular triangle read by rows
 * where row m lists the prime indices of the m‑th square‑free number.
 * Computation: iterate over square‑free numbers in increasing order,
 * factor each using a precomputed smallest‑prime‑factor array,
 * and output their prime indices in ascending order.
 */

#include <stdbool.h>
#include <stdint.h>

#define LIMIT 200000          /* enough for n up to at least 10 000 */

static int prime_index[LIMIT + 1];
static int spf[LIMIT + 1];
static bool is_composite[LIMIT + 1];
static bool sieve_done = false;

static void init_sieve(void)
{
    if (sieve_done) return;
    /* initialise spf and composite flag */
    for (int i = 0; i <= LIMIT; ++i) {
        spf[i] = i;
        is_composite[i] = false;
    }
    int pc = 0;   /* prime counter (1‑based) */

    for (int i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {         /* i is prime */
            ++pc;
            prime_index[i] = pc;
            if ((int64_t)i * i <= LIMIT) {
                for (int j = i * i; j <= LIMIT; j += i) {
                    if (!is_composite[j]) {
                        is_composite[j] = true;
                        if (spf[j] == j) spf[j] = i;
                    }
                }
            }
        }
    }
    sieve_done = true;
}

seq_t a(idx_t n)
{
    init_sieve();   /* ensure prime tables are ready */

    seq_t counter = 0;             /* how many terms produced so far */
    int indices[32];               /* store indices of prime factors of current number */

    for (idx_t val = 2; ; ++val) {
        if (val > LIMIT) {         /* should never happen for the tested n */
            break;
        }

        /* factor val using spf, check square‑free and collect prime indices */
        idx_t temp = val;
        int last = 0;
        int k = 0;
        bool squarefree = true;
        while (temp > 1) {
            int p = spf[(int)temp];
            if (p == last) {        /* repeated factor → not square‑free */
                squarefree = false;
                break;
            }
            indices[k++] = prime_index[p];
            last = p;
            temp /= p;
        }
        if (!squarefree) continue;

        /* append the collected indices to the sequence */
        for (int i = 0; i < k; ++i) {
            ++counter;
            if (counter == n) {
                return (seq_t)indices[i];
            }
        }
    }
    return 0;   /* unreachable for valid n */
}
