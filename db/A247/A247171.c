/*
 * A247171  G.f.: (2*x^2+4*x+3)/((2*x+2)*sqrt(-4*x^3-4*x^2+1))-1/(2*x+2).
 * https://oeis.org/A247171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*Sum_{k=1..n} (binomial(2*k,n-k)*binomial(n-k-1,k-1))/k,
 *            n>0, a(0)=1. Recurrence:
 *            3*n*a(n)+(7*n-8)*a(n-1)+2*(-3*n-2)*a(n-2)+2*(-19*n+35)*a(n-3)+2*(-26*n+81)*a(n-4)+4*(-8*n+35)*a(n-5)+4*(-2*n+11)*a(n-6)=0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    idx_t need = n + 1;
    if (cache_len < need)
    {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        cache = tmp;
        for (idx_t i = cache_len; i < need; ++i)
            cache[i] = (seq_t)0;
        cache_len = need;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t start = cache_len;
    ensure_cache(n);

    for (idx_t i = start; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else if (i == 3)
            cache[i] = (seq_t)4;
        else if (i == 4)
            cache[i] = (seq_t)11;
        else if (i == 5)
            cache[i] = (seq_t)21;
        else
        {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t a6 = cache[i - 6];

            seq_t n_seq = (seq_t)i;

            seq_t term1 = ((seq_t)7 * n_seq - (seq_t)8) * a1;
            seq_t term2 = ((seq_t)2 * ((seq_t)3 * n_seq + (seq_t)2)) * a2;
            seq_t term3 = ((seq_t)2 * ((seq_t)19 * n_seq - (seq_t)35)) * a3;
            seq_t term4 = ((seq_t)2 * ((seq_t)26 * n_seq - (seq_t)81)) * a4;
            seq_t term5 = ((seq_t)4 * ((seq_t)8 * n_seq - (seq_t)35)) * a5;
            seq_t term6 = ((seq_t)4 * ((seq_t)2 * n_seq - (seq_t)11)) * a6;

            seq_t sum = term2;
            sum += term3;
            sum += term4;
            sum += term5;
            sum += term6;

            seq_t numerator = sum - term1;
            seq_t denom = (seq_t)3 * n_seq;
            cache[i] = numerator / denom;
        }
    }

    return cache[n];
}
