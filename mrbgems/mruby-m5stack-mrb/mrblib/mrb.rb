class MRB
  COLMAX = 4
  ROWMAX = 26

  @@mrbs = nil

  def self.showlist(mrbs)
    mrbs.each_with_index {|mrb, i|
      # print sprintf("%3d:%-8s", i+1, mrb)
      print ("  " + (i + 1).to_s)[-3..-1] + ":" + (mrb + "       ")[0..7]
      puts "" if (i + 1) % @@colmax == 0 || i == mrbs.size - 1
    }
  end

  def self.menu
    @@fontsize = 1
    @@mrbs = list.sort unless @@mrbs
    page = 0

    loop {
      @@colmax = COLMAX / @@fontsize
      @@rowmax = ROWMAX / @@fontsize
      mrb = nil

      # puts @@fontsize, @@colmax, @@rowmax

      sidx = page * @@colmax * @@rowmax
      eidx = (page + 1) * @@colmax * @@rowmax - 1
      eidx = @@mrbs.size - 1 if @@mrbs.size <= eidx

      # mrblist at page
      mrbs = @@mrbs[sidx..eidx]

      # show mrb list
      showlist(mrbs)

      cmd = nil
      loop {
        print ">>> "
        cmd = gets.strip
        break if cmd.size > 0
      }

      case cmd.upcase
      when "N"  # next page
        page += 1 if @@mrbs.size <= eidx + 1
      when "P"  # previous page
        page -= 1 if page > 0
      when "Z"  # change font size
        @@fontsize ^= 3 # 1<=>2
        @@colmax = COLMAX / @@fontsize
        @@rowmax = ROWMAX / @@fontsize
        puts "fontsize=#{@@fontsize}"
      when "Q"  # quit
        break
      when "?"  # help
        puts <<"EOS"
Commands
  1..#{@@mrbs.size}: Run application specified by the number.
  "app": Run application with specified name.
  N: Show next page.
  P: Show previous page.
  Q: Quit menu.
  Z: Change font size.
  ?: Show this help.

EOS
      else      # appno->mrbidx
        mrbidx = cmd.to_i
        mrbidx = 0 if @@mrbs.size <= mrbidx
        if mrbidx > 0 # number
          mrbidx = mrbidx + page * @@colmax * @@rowmax - 1
          # puts "mrbidx = #{mrbidx}"
          mrb = @@mrbs[mrbidx]
        else  # not number
          if (cmd[0] == "\"" && cmd[-1] == "\"") ||
             (cmd[0] == "\'" && cmd[-1] == "\'")
             mrb = cmd[1..-2]
             unless @@mrbs.any? {|m| m.upcase == mrb.upcase}
              puts "`#{mrb}` is not found."
              mrb = nil
             end
          end
        end
        if mrb
          print "Run `#{mrb}`? (Yes/No) \n=> "
          yn = gets.strip.upcase
          if yn[0] == "Y"
            # Run target mrb
            run(mrb)
          end
        end
      end
    }
  end
end
