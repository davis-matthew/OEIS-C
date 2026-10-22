/*
 * A357050  Number of ways A005101(n)+1 can be written as sum of a subset of
 *          the proper divisors of A005101(n), the n-th abundant number.
 * https://oeis.org/A357050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: for n ≥ 1 let N be the n‑th abundant number A005101(n).
   Return the number of subsets of the proper divisors of N whose sum
   equals N+1.  The function rebuilds the abundant list on demand and
   uses a classic subset‑sum DP. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- elementary factorisation ------------------------------------------*/
static size_t factorize(uint64_t n, uint64_t primes[], int exps[])
{
    size_t cnt = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]  = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/*--- recursive generation of all divisors -----------------------------*/
static void
gen_divisors(const uint64_t *pr, const int *ex, size_t pc, size_t i,
             uint64_t cur, uint64_t **arr, size_t *cnt, size_t *cap)
{
    if (i == pc) {
        if (*cnt == *cap) {
            *cap = (*cap ? *cap * 2 : 4);
            *arr = (uint64_t *)realloc(*arr, *cap * sizeof(uint64_t));
        }
        (*arr)[(*cnt)++] = cur;
        return;
    }
    uint64_t acc = 1;
    for (int e = 0; e <= ex[i]; ++e) {
        gen_divisors(pr, ex, pc, i + 1, cur * acc, arr, cnt, cap);
        acc *= pr[i];
    }
}

/*--- main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    /*--- 1. Build (or extend) the list of the first n abundant nums---*/
    static uint64_t *abundant = NULL;
    static size_t abundant_cnt = 0;
    static size_t abundant_cap = 0;
    static uint64_t last_checked = 12;   /* first candidate */

    if (n > abundant_cnt) {
        size_t need = n;
        if (abundant_cap < need) {
            size_t new_cap = abundant_cap ? abundant_cap * 2 : 32;
            while (new_cap < need) new_cap *= 2;
            abundant = (uint64_t *)realloc(abundant, new_cap * sizeof(uint64_t));
            abundant_cap = new_cap;
        }
        for (; abundant_cnt < need; ++last_checked) {
            uint64_t primes[64];
            int exps[64];
            size_t pf_cnt = factorize(last_checked, primes, exps);

            unsigned __int128 sigma = 1;
            for (size_t i = 0; i < pf_cnt; ++i) {
                unsigned __int128 p_pow = 1;
                for (int j = 0; j < exps[i] + 1; ++j) p_pow *= primes[i];
                unsigned __int128 term = (p_pow - 1) / (primes[i] - 1);
                sigma *= term;
                if (sigma > (unsigned __int128)2 * last_checked) break;
            }
            if (sigma > (unsigned __int128)2 * last_checked) {
                abundant[abundant_cnt++] = last_checked;
            }
        }
    }

    /*--- 2. Retrieve the n-th abundant number ------------------------*/
    uint64_t N = abundant[n - 1];
    seq_t target = (seq_t)(N + 1);

    /*--- 3. Generate all proper divisors of N ------------------------*/
    uint64_t primes[64];
    int exps[64];
    size_t pf_cnt = factorize(N, primes, exps);

    uint64_t *divs = NULL;
    size_t div_cnt = 0, div_cap = 0;
    gen_divisors(primes, exps, pf_cnt, 0, 1ULL, &divs, &div_cnt, &div_cap);

    /*--- 4. Subset‑sum DP ------------------------------------------------*/
    size_t tgt = (size_t)target;
    seq_t *dp = (seq_t *)calloc(tgt + 1, sizeof(seq_t));
    dp[0] = (seq_t)1;

    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        if (d == N) continue;          /* exclude N itself */
        size_t ds = (size_t)d;
        for (size_t s = tgt; s >= ds; --s) {
            dp[s] += dp[s - ds];
        }
    }

    seq_t result = dp[tgt];
    free(dp);
    free(divs);
    return result;
}
