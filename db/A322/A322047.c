/*
 * A322047  Numbers which when written in Finnish do not contain the letter
 *          "e".
 * https://oeis.org/A322047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n-th number whose Finnish word contains no letter “e”.  
   The sequence is non‑increasing in complexity: for each integer we
   examine its Finnish representation (units, teens, tens, hundreds,
   thousands) and reject it if any part contains “e”. */
#include <stdbool.h>

/* Whether the Finnish word for a digit contains the letter 'e'. */
static const bool units_has_e[10] = {
    false, /* 0: nolla      */
    false, /* 1: yksi       */
    false, /* 2: kaksi      */
    true,  /* 3: kolme      */
    true,  /* 4: neljä      */
    false, /* 5: viisi      */
    false, /* 6: kuusi      */
    true,  /* 7: seitsemän  */
    true,  /* 8: kahdeksan  */
    true   /* 9: yhdeksän   */
};

/* Whether the Finnish word for a number 10‑19 contains 'e'. */
static const bool teens_has_e[10] = {
    true,  /* 10: kymmenen          */  /* contains 'e' */
    false, /* 11: yksitoista       */
    false, /* 12: kaksitoista      */
    true,  /* 13: kolmistoista     */  /* contains 'e' */
    true,  /* 14: neljätoista      */  /* contains 'e' */
    false, /* 15: viisitoista      */
    false, /* 16: kuusitoista      */
    true,  /* 17: seitsemäntoista   */  /* contains 'e' */
    true,  /* 18: kahdeksantoista  */  /* contains 'e' */
    true   /* 19: yhdeksäntoista   */  /* contains 'e' */
};

/* Whether the Finnish word for a tens digit (20–90) contains 'e'. */
static const bool tens_has_e[10] = {
    false, /* 0: (no word)          */
    false, /* 1: (handled by teens) */
    true,  /* 2: kaksikymmentä     */
    true,  /* 3: kolmekymmentä     */
    true,  /* 4: neljäkymmentä     */
    true,  /* 5: viisikymmentä     */
    true,  /* 6: kuusikymmentä     */
    true,  /* 7: seitsemänkymmentä */
    true,  /* 8: kahdeksankymmentä */
    true   /* 9: yhdeksänkymmentä  */
};

/* Whether the Finnish word for a hundreds digit (100‑900) contains 'e'. */
static const bool hundreds_has_e[10] = {
    false, /* 0: (no word)            */
    false, /* 1: sata                   */
    false, /* 2: kaksisata              */
    true,  /* 3: kolmesata              */  /* contains 'e' */
    true,  /* 4: neljäsata              */  /* contains 'e' */
    false, /* 5: viisisata              */
    false, /* 6: kuusisata              */
    true,  /* 7: seitsemänsata          */  /* contains 'e' */
    true,  /* 8: kahdeksansata          */  /* contains 'e' */
    true   /* 9: yhdeksänsata           */  /* contains 'e' */
};

/* Whether the multiplier word for a group of thousands (tuhat, miljoona, …)
   contains 'e'. All are free of 'e' in Finnish. */
static const bool multiplier_has_e[4] = {
    false, /* 0: unused       */
    false, /* 1: tuhatus     */
    false, /* 2: miljoona    */
    false  /* 3: miljoardi   */
};

/* Return true if the Finnish name of ‘n’ does not contain the letter 'e'. */
static bool no_e_in_finnish(idx_t n)
{
    if (n == 0)
        return true;                     /* “nolla” has no 'e' */

    idx_t temp = n;
    int   group_idx = 0;                  /* 0: units, 1: thousands, etc. */

    while (temp > 0) {
        idx_t group = temp % 1000;        /* 0–999 */
        if (group != 0) {
            /* Hundreds part */
            idx_t h   = group / 100;
            idx_t rem = group % 100;
            if (h > 0 && hundreds_has_e[(int)h])
                return false;

            /* Tens / units part */
            if (rem > 0) {
                if (rem == 10)
                    return false;           /* “kymmenen” contains 'e' */
                if (rem < 10) {
                    if (units_has_e[(int)rem])
                        return false;
                } else if (rem < 20) {       /* 11‑19 */
                    if (teens_has_e[(int)(rem - 10)])
                        return false;
                } else {                     /* 20‑99: tens + units */
                    idx_t t = rem / 10;          /* 2–9 */
                    idx_t u = rem % 10;
                    if (tens_has_e[(int)t])
                        return false;
                    if (u > 0 && units_has_e[(int)u])
                        return false;
                }
            }
            /* Multiplier word (e.g. “tuhat”) - only if it applies */
            if (group_idx > 0) {
                if (group_idx >= 1 && group_idx <= 3) {
                    if (multiplier_has_e[group_idx])
                        return false;
                }
                /* For higher multipliers we assume no 'e'. */
            }
        }
        temp /= 1000;
        ++group_idx;
    }
    return true;
}

/* Return the n-th element (1‑indexed) of sequence A322047. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t num = 0;; ++num) {
        if (no_e_in_finnish(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
}
