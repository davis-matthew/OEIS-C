/*
 * A360710  Multiplicative with a(p^k) = 1 or -1 so as to minimize abs(Sum_{m
 *          = 1..p^k} a(m)); in case of a tie, a(p^k) = a(p^k-1).
 * https://oeis.org/A360710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative; for each prime power p^k, a(p^k) = 1 or
 *            –1 chosen to minimize |∑_{m≤p^k} a(m)|. If the minimum is
 *            attained by both signs, the rule a(p^k) = a(p^k–1) is used
 * Range:     i8 n <= 6328, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* maximum n we pre‑compute – the sequence is required only up to
   about 10000, so a 100000 limit is more than sufficient. */
#define MAXN 100000

/* static state – initialised only on the first call */
static seq_t        cache[MAXN + 1];
static seq_t        pref [MAXN + 1];
static uint32_t     spf   [MAXN + 1];
static bool         init  = false;
static idx_t        next_computed = 1;   /* next index to be computed (1‑based) */

static void build_spf(void)
{
    for (uint32_t i = 0; i <= MAXN; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= MAXN; ++i)
        if (spf[i] == 0)
        {
            spf[i] = i;
            if ((uint64_t)i * i <= MAXN)
                for (uint32_t j = i * i; j <= MAXN; j += i)
                    if (spf[j] == 0) spf[j] = i;
        }
}

/* minimal absolute value helper */
static seq_t abs_seq(seq_t x) { return x >= 0 ? x : -x; }

seq_t a(idx_t n)
{
    if (n < 1 || n > MAXN) return 0;   /* out of bounds → 0 */

    if (!init)
    {
        /* initialise prefix[0] */
        pref[0] = 0;
        /* flag that the memoised table is ready */
        init = true;
        /* build smallest prime factor table once */
        build_spf();
    }

    /* compute pending values up to n */
    while (next_computed <= n)
    {
        idx_t i = next_computed;

        if (i == 1)
        {
            cache[1] = 1;
            pref[1]  = 1;
        }
        else
        {
            /* factor i using spf */
            idx_t temp = i;
            int  fp[10];   /* distinct prime factors */
            int  ex[10];   /* their exponents */
            int  cnt = 0;
            while (temp > 1 && cnt < 10)
            {
                uint32_t p = spf[temp];
                int e = 0;
                while (temp % p == 0)
                {
                    temp /= p;
                    ++e;
                }
                fp[cnt] = (int)p;
                ex[cnt] = e;
                ++cnt;
            }

            if (cnt == 1)
            {
                /* i is a prime power p^e */
                seq_t sum_plus  = pref[i - 1] + 1;
                seq_t sum_minus = pref[i - 1] - 1;
                seq_t abs_plus  = abs_seq(sum_plus);
                seq_t abs_minus = abs_seq(sum_minus);

                if (abs_plus < abs_minus)
                    cache[i] = 1;
                else if (abs_minus < abs_plus)
                    cache[i] = -1;
                else
                    cache[i] = cache[i - 1];   /* tie → previous sign */
            }
            else
            {
                /* composite with at least two distinct primes – multiplicative */
                seq_t val = 1;
                for (int k = 0; k < cnt; ++k)
                {
                    /* compute p^e */
                    idx_t pow = 1;
                    for (int t = 0; t < ex[k]; ++t) pow *= (idx_t)fp[k];
                    val *= cache[pow];
                }
                cache[i] = val;
            }

            pref[i] = pref[i - 1] + cache[i];
        }

        ++next_computed;
    }

    return cache[n];
}
