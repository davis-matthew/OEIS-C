/*
 * A352554  a(n) = number of modules with n elements over the ring of integers
 *          in the real quadratic field of discriminant 17.
 * https://oeis.org/A352554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count the number of modules of size n over the ring
   of integers in the real quadratic field Q(√17).  The ring is a
   PID with discriminant 17.  For each rational prime p dividing n,
   the module structure is determined by the splitting behaviour of p:
      * p = 17                (ramified) : only one prime ideal of norm 17.
      * p = 2                 (splits)  : two prime ideals each of norm 2.
      * p odd:
           * (17|p) =  1      (splits)  : two prime ideals each of norm p.
           * (17|p) = -1      (inert)  : one prime ideal of norm p².
   The number of modules of size p^k is:
      * ramified:  partitions of k  →  P(k)
      * split   :  sum_{a+b=k} P(a)·P(b)  →  convolution of partitions.
      * inert   :  if k odd → 0;  if k even → partitions of k/2  →  P(k/2).
   The total number for n is the product of these local counts over all
   distinct rational primes dividing n.  If any local count is zero,
   the whole product is zero.  Partition numbers P(m) are computed by
   Euler's recurrence.  Convolution is computed multiplicatively and
   runs in O(e_max) time.  The algorithm is O(√n) for factorisation
   plus linear in the maximum exponent that occurs. */

#include <stdint.h>
#include <stdbool.h>

static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /*--- factorisation of n ----------------------------------------*/
    uint64_t N = n;
    if (N <= 1) return (seq_t)1;   /* n = 1 gives one trivial module */

    uint64_t primes[10];
    int exps[10];
    int pf_cnt = 0;
    uint64_t tmp = N;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1)
    {
        primes[pf_cnt] = tmp;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /*--- determine maximal exponent needed for partitions ----------*/
    int max_exp_needed = 0;
    for (int i = 0; i < pf_cnt; ++i)
    {
        uint64_t p = primes[i];
        int e = exps[i];
        if (p == 17 || p == 2)                /* ramified or split 2   */
        {
            if (e > max_exp_needed) max_exp_needed = e;
        }
        else                                  /* odd primes           */
        {
            uint64_t leg = pow_mod_uint64(17 % p, (p - 1) / 2, p);
            if (leg == 1)                     /* splits               */
            {
                if (e > max_exp_needed) max_exp_needed = e;
            }
            else                              /* inert                */
            {
                int ee = e / 2;
                if (ee > max_exp_needed) max_exp_needed = ee;
            }
        }
    }

    /*--- compute partition numbers p[0..max_exp_needed] -------------*/
    static seq_t part[64];   /* 64 > any needed exponent for n <= 10000   */
    part[0] = (seq_t)1;
    for (int k = 1; k <= max_exp_needed; ++k)
    {
        long long sum = 0;
        for (int m = 1; ; ++m)
        {
            int p1 = m * (3 * m - 1) / 2;
            if (p1 > k) break;
            int p2 = m * (3 * m + 1) / 2;
            int sign = (m & 1) ? 1 : -1;
            sum += sign * (long long)part[k - p1];
            if (p2 <= k) sum += sign * (long long)part[k - p2];
        }
        part[k] = (seq_t)sum;
    }

    /*--- convolution of partitions for split primes --------------*/
    static seq_t conv[64];
    conv[0] = (seq_t)1;
    for (int e = 1; e <= max_exp_needed; ++e)
    {
        long long cs = 0;
        for (int a = 0; a <= e; ++a)
            cs += (long long)part[a] * (long long)part[e - a];
        conv[e] = (seq_t)cs;
    }

    /*--- assemble result over all prime factors -----------------*/
    seq_t result = (seq_t)1;
    for (int i = 0; i < pf_cnt; ++i)
    {
        uint64_t p = primes[i];
        int e = exps[i];
        seq_t local = (seq_t)0;

        if (p == 17)                      /* ramified prime */
        {
            local = part[e];
        }
        else if (p == 2)                  /* 2 splits */
        {
            local = conv[e];
        }
        else                               /* odd primes */
        {
            uint64_t leg = pow_mod_uint64(17 % p, (p - 1) / 2, p);
            if (leg == 1)                  /* splits       */
            {
                local = conv[e];
            }
            else if (leg == p - 1)         /* inert        */
            {
                if (e & 1)                /* odd exponent impossible */
                    return (seq_t)0;
                local = part[e / 2];
            }
            else                           /* should not happen */
                return (seq_t)0;
        }

        result *= local;
    }

    return result;
}
