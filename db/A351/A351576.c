/*
 * A351576  Factorial base expansion of n reinterpreted as a primorial base
 *          expansion, then converted back to decimal.
 * https://oeis.org/A351576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A276085(A276076(n)) (factorial‑base digits interpreted in
 *            primorial base)
 * Range:     u8 n <= 159, u16 n <= 8355, u32 n <= 8355, u64 n <= 8355,
 *            u128 n <= 8355, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* first 128 primes; index 0 corresponds to prime 2 */
static const uint64_t primes_list[] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,
    73ULL,79ULL,83ULL,89ULL,97ULL,101ULL,103ULL,107ULL,109ULL,113ULL,
    127ULL,131ULL,137ULL,139ULL,149ULL,151ULL,157ULL,163ULL,167ULL,173ULL,
    179ULL,181ULL,191ULL,193ULL,197ULL,199ULL,211ULL,223ULL,227ULL,229ULL,
    233ULL,239ULL,241ULL,251ULL,257ULL,263ULL,269ULL,271ULL,277ULL,281ULL,
    283ULL,293ULL,307ULL,311ULL,313ULL,317ULL,331ULL,337ULL,347ULL,349ULL,
    353ULL,359ULL,367ULL,373ULL,379ULL,383ULL,389ULL,397ULL,401ULL,409ULL,
    419ULL,421ULL,431ULL,433ULL,439ULL,443ULL,449ULL,457ULL,461ULL,463ULL,
    467ULL,479ULL,487ULL,491ULL,499ULL,503ULL,509ULL,521ULL,523ULL,541ULL,
    547ULL,557ULL,563ULL,569ULL,571ULL,577ULL,587ULL,593ULL,599ULL,601ULL,
    607ULL,613ULL,617ULL,619ULL,631ULL,641ULL,643ULL,647ULL,653ULL,659ULL,
    661ULL,673ULL,677ULL,683ULL,691ULL,701ULL,709ULL,719ULL,727ULL,733ULL,
    739ULL,743ULL,751ULL
};

#define MAX_M 128  /* maximum factorial depth supported */

/* Compute a(n) = A276085(A276076(n)) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find the largest factorial <= n */
    size_t m   = 1;                     /* factorial index (i) */
    seq_t fact = 1;                      /* i! */
    while (1)
    {
        seq_t next = fact * (seq_t)(m + 1);   /* (i+1)! */
        if (next > n)
            break;
        fact = next;
        m++;
    }

    /* Compute primorials up to index m-1 */
    seq_t prim[MAX_M];
    prim[0] = 1;                          /* product of 0 primes = 1 */
    for (size_t j = 1; j < m; ++j)
        prim[j] = prim[j - 1] * (seq_t)primes_list[j - 1];

    /* Compute digits of n in factorial base and accumulate result */
    seq_t temp   = (seq_t)n;
    seq_t result = 0;
    seq_t cur_fact = fact;                /* current factorial (i!) */
    for (size_t i = m; i > 0; --i)
    {
        seq_t digit = temp / cur_fact;
        temp -= digit * cur_fact;
        result += digit * prim[i - 1];
        if (i == 1)
            break;
        cur_fact /= (seq_t)i;            /* (i-1)! */
    }

    return result;
}
