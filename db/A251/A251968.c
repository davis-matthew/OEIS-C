/*
 * A251968  Number of (n+1) X (n+1) 0..3 arrays with nondecreasing sum of
 *          every two consecutive values in every row and column.
 * https://oeis.org/A251968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = M(r_ev,r_ev) * M(r_od,r_od) * M(r_ev,r_od)^2 where M(r,c)
 *            = ∏_{i=1}^{r}∏_{j=1}^{c} (i+j+2)/(i+j-1) gives the number of r×c
 *            matrices with entries 0..3 that are non‑decreasing in each row
 *            and column
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* include the provided power routine for seq_t */
#include "../utils/pow_seq.h"

/* ---------------------------------------------------------------------- */
/* prime precomputation                                                    */
/* ---------------------------------------------------------------------- */
#define MAXP 10005        /* largest value needed (sum+2) ≤ rows+cols+2 ≤ 10004 for n≤10000 */
#define MAXPL 2000        /* enough to hold all primes ≤ MAXP */

static int spf[MAXP];              /* smallest prime factor for each integer */
static int prime_index[MAXP];      /* index of prime in primes array, -1 for composites */
static int primes[MAXPL];
static size_t primes_cnt = 0;
static bool primes_built = false;

static void build_primes(void)
{
    size_t i, j;

    for (i = 0; i < MAXP; ++i) {
        spf[i] = 0;
        prime_index[i] = -1;
    }

    for (i = 2; i < MAXP; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (int)i;
            prime_index[i] = (int)primes_cnt;
            primes[primes_cnt++] = (int)i;
        }
        for (j = 0; j < primes_cnt; ++j) {
            int p = primes[j];
            if (p > spf[i] || i * (size_t)p >= MAXP) break;
            spf[i * p] = p;
        }
    }
    primes_built = true;
}

/* ---------------------------------------------------------------------- */
/* helper: add the factorisation of num to the exponent array            */
/* ---------------------------------------------------------------------- */
static void add_factor(int num, int exp_mult, int *exps)
{
    while (num > 1) {
        int p = spf[num];
        int cnt = 0;
        while (num % p == 0) {
            num /= p;
            ++cnt;
        }
        int idx = prime_index[p];
        exps[idx] += exp_mult * cnt;
    }
}

/* ---------------------------------------------------------------------- */
/* count_submatrix(rows, cols) – number of rows×cols matrices with        */
/* entries 0..3 that are monotone non‑decreasing in rows and columns.     */
/* ---------------------------------------------------------------------- */
static seq_t count_submatrix(idx_t rows, idx_t cols)
{
    int exps[MAXPL];
    size_t i;

    /* reset exponents */
    for (i = 0; i < primes_cnt; ++i) exps[i] = 0;

    idx_t max_sum = rows + cols;          /* maximum i+j value */
    for (idx_t s = 2; s <= max_sum; ++s) {
        idx_t low  = (s > cols) ? (s - cols) : 1;
        idx_t high = (s - 1 < rows) ? (s - 1) : rows;
        if (low > high) continue;
        idx_t N = high - low + 1;        /* number of (i,j) with i+j=s */

        int num = (int)(s + 2);
        int den = (int)(s - 1);

        add_factor(num, (int)N, exps);
        add_factor(den, -(int)N, exps);
    }

    seq_t res = 1;
    for (i = 0; i < primes_cnt; ++i) {
        int e = exps[i];
        if (e > 0) {
            res *= pow_seq((seq_t)primes[i], e);
        }
    }
    return res;
}

/* ---------------------------------------------------------------------- */
/* public API: a(n)                                                        */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the sequence range */

    if (!primes_built) build_primes();

    /* parity class sizes */
    idx_t even_rows = n / 2 + 1;          /* floor(n/2)+1 */
    idx_t odd_rows  = (n + 1) / 2;        /* ceil(n/2)   */

    seq_t c11 = count_submatrix(even_rows, even_rows);
    seq_t c22 = count_submatrix(odd_rows,  odd_rows);
    seq_t c12 = count_submatrix(even_rows, odd_rows);   /* equals count_submatrix(odd_rows,even_rows) */

    return c11 * c22 * c12 * c12;
}
