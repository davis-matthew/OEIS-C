/*
 * A363832  Number of digits left of the radix point of n when written in base
 *          e using a greedy algorithm representation.
 * https://oeis.org/A363832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(log_e(n)) + 1
 * Range:     u8 n <= 10, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ceil(e^k) for k = 0 … 44.  All values fit in 64‑bit, so they
       can be written as integer literals and implicitly converted to
       the build's seq_t type. */
    static const seq_t t[] = {
        1ULL,                     /* ceil(e^0)  = 1   */
        3ULL,                     /* ceil(e^1)  = 3   */
        8ULL,                     /* ceil(e^2)  = 8   */
        21ULL,                    /* ceil(e^3)  = 21  */
        55ULL,                    /* ceil(e^4)  = 55  */
        149ULL,                   /* ceil(e^5)  = 149 */
        404ULL,                   /* ceil(e^6)  = 404 */
        1097ULL,                  /* ceil(e^7)  = 1097*/
        2981ULL,                  /* ceil(e^8)  = 2981*/
        8104ULL,                  /* ceil(e^9)  = 8104*/
        22027ULL,                 /* ceil(e^10) = 22027*/
        59875ULL,                 /* ceil(e^11) = 59875*/
        162755ULL,                /* ceil(e^12) = 162755*/
        442414ULL,                /* ceil(e^13) = 442414*/
        1202605ULL,               /* ceil(e^14) = 1202605*/
        3269018ULL,               /* ceil(e^15) = 3269018*/
        8886111ULL,               /* ceil(e^16) = 8886111*/
        24154953ULL,              /* ceil(e^17) = 24154953*/
        65659970ULL,              /* ceil(e^18) = 65659970*/
        178482406ULL,             /* ceil(e^19) = 178482406*/
        483165954ULL,             /* ceil(e^20) = 483165954*/
        1314813996ULL,            /* ceil(e^21) = 1314813996*/
        3583462864ULL,            /* ceil(e^22) = 3583462864*/
        9765857788ULL,            /* ceil(e^23) = 9765857788*/
        26527428366ULL,           /* ceil(e^24) = 26527428366*/
        72024155654ULL,           /* ceil(e^25) = 72024155654*/
        195963106051ULL,          /* ceil(e^26) = 195963106051*/
        534686554760ULL,          /* ceil(e^27) = 534686554760*/
        1461449491366ULL,         /* ceil(e^28) = 1461449491366*/
        4016714930677ULL,         /* ceil(e^29) = 4016714930677*/
        11025842318599ULL,        /* ceil(e^30) = 11025842318599*/
        30209555443076ULL,        /* ceil(e^31) = 30209555443076*/
        82777015219397ULL,        /* ceil(e^32) = 82777015219397*/
        227837313560556ULL,       /* ceil(e^33) = 227837313560556*/
        626074030464091ULL,       /* ceil(e^34) = 626074030464091*/
        1727509869734263ULL,      /* ceil(e^35) = 1727509869734263*/
        4759674643170722ULL,      /* ceil(e^36) = 4759674643170722*/
        13124031946023201ULL,     /* ceil(e^37) = 13124031946023201*/
        36241392821408201ULL,     /* ceil(e^38) = 36241392821408201*/
        100053203310386202ULL,    /* ceil(e^39) = 100053203310386202*/
        276719930869115202ULL,    /* ceil(e^40) = 276719930869115202*/
        765526780876404801ULL,    /* ceil(e^41) = 765526780876404801*/
        2080917537655080324ULL,   /* ceil(e^42) = 2080917537655080324*/
        5656520329129545375ULL,   /* ceil(e^43) = 5656520329129545375*/
        15376016422982019656ULL   /* ceil(e^44) = 15376016422982019656*/
    };

    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* Find the largest k with ceil(e^k) <= n.  This k is floor(log_e(n)). */
    idx_t low = 0;
    idx_t high = sizeof(t) / sizeof(t[0]) - 1;
    idx_t m = 0;                /* floor(log_e(n)) */

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (t[mid] <= (seq_t)n) {
            m = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* a(n) = floor(log_e(n)) + 1 */
    return (seq_t)(m + 1);
}
