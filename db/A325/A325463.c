/*
 * A325463  Complement of A325462.
 * https://oeis.org/A325463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth number in the set defined by the recursive rule:
 *            n is in the sequence iff (n is even and n/2 is NOT in the
 *            sequence) OR (n ≡ 2 (mod 3) and (n+1)/3 is NOT in the sequence).
 *            The first few terms are: 2,6,8,10,11,14,18,
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n = 0 */

    /* dynamic bit array: 1 = in sequence, 0 = not */
    size_t alloc      = 2;                /* indices 0 .. 1 */
    unsigned char *in_a = (unsigned char *)malloc(alloc * sizeof(unsigned char));
    if (!in_a)
        return 0;
    in_a[1] = 0;                        /* 1 is not in the sequence */

    size_t count = 0;                    /* how many terms found */
    idx_t cur   = 1;                     /* current integer to test */

    while (count < (size_t)n)
    {
        /* determine if cur belongs to the sequence */
        unsigned char val = 0;
        if ((cur & 1) == 0)                 /* cur even */
        {
            size_t idx = (size_t)(cur / 2);
            if (idx >= alloc)
            {
                size_t new_alloc = alloc * 2;
                unsigned char *tmp = (unsigned char *)realloc(in_a,
                                                            new_alloc * sizeof(unsigned char));
                if (!tmp)
                {
                    free(in_a);
                    return 0;
                }
                for (size_t i = alloc; i < new_alloc; ++i)
                    tmp[i] = 0;
                in_a = tmp;
                alloc = new_alloc;
            }
            if (!in_a[idx])
                val = 1;
        }

        if (!val && cur % 3 == 2)           /* cur ≡ 2 (mod 3) */
        {
            idx_t t = (cur + 1) / 3;
            size_t idx = (size_t)t;
            if (idx >= alloc)
            {
                size_t new_alloc = alloc * 2;
                unsigned char *tmp = (unsigned char *)realloc(in_a,
                                                            new_alloc * sizeof(unsigned char));
                if (!tmp)
                {
                    free(in_a);
                    return 0;
                }
                for (size_t i = alloc; i < new_alloc; ++i)
                    tmp[i] = 0;
                in_a = tmp;
                alloc = new_alloc;
            }
            if (!in_a[idx])
                val = 1;
        }

        /* ensure the array covers cur */
        if ((size_t)cur >= alloc)
        {
            size_t new_alloc = alloc * 2;
            unsigned char *tmp = (unsigned char *)realloc(in_a,
                                                        new_alloc * sizeof(unsigned char));
            if (!tmp)
            {
                free(in_a);
                return 0;
            }
            for (size_t i = alloc; i < new_alloc; ++i)
                tmp[i] = 0;
            in_a = tmp;
            alloc = new_alloc;
        }

        in_a[(size_t)cur] = val;

        if (val)
        {
            ++count;
            if (count == (size_t)n)
            {
                free(in_a);
                return (seq_t)cur;
            }
        }

        ++cur;
    }

    free(in_a);          /* should never reach */
    return 0;
}
